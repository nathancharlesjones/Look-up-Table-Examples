#! /bin/bash

# Copyright (c) 2004-2007 Juan M. Bello Rivas <jmbr@superadditive.com>

prog_name="callgraph"

if [ $# -lt 1 ]; then
    echo "Usage: $prog_name EXECUTABLE [ARGS...]"
    echo
    echo "Example: $prog_name ~/bin/test-program foo 23"
    exit 1
fi

# Sanity checks.
FILE=$1

if [ ! -x $FILE ]; then
    echo "$prog_name: Unable to find executable '$FILE'"
    exit 1
fi

staticBin=`file -L $FILE`

if [[ "$staticBin" =~ " ELF " ]]; then
    if [[ "$staticBin" =~ "statically linked" ]]; then
      staticBin=1
    else
      staticBin=0
    fi
else
    echo "$prog_name: '$FILE' is not an ELF executable binary"
    exit 1
fi

shift

# Set up temporary files.
TRACE="`mktemp -t $prog_name.XXXXXXXXXX`" || exit
GETFUNCS="`mktemp -t $prog_name.XXXXXXXXXX`" || exit
trap 'rm -f -- "$TRACE" "$GETFUNCS"' EXIT
trap 'trap - EXIT; rm -f -- "$TRACE" "$GETFUNCS"; exit 1' HUP INT QUIT TERM

echo $TRACE
echo $GETFUNCS

# Find the Entry Point Address
entryAddr=$(readelf -h $FILE | awk ' /Entry point address/ { print "tbreak *" $4 } ')

# Take control of GDB and print call graph.
cat > $GETFUNCS <<EOF
    set width 0
    set height 0
    set pagination off
    set verbose off
    set confirm off
    set disable-randomization on
    set demangle-style none
    set print demangle off
EOF
echo $entryAddr >> $GETFUNCS
echo "start" >> $GETFUNCS
echo "info function" >> $GETFUNCS
nm  $FILE |awk ' $2 ~ /^[tT]$/ { print "echo 0x" $1 " " $3 "\\n" } ' >> $GETFUNCS
nm -D $FILE |awk ' $2 ~ /^[tT]$/ { print "echo 0x" $1 " " $3 "\\n" } ' >> $GETFUNCS

cat $GETFUNCS

# For dynamically linked binaries, we first set a temporary
# breakpoint on several possible Entry Point Addresses and
# run until it stops. Only after that do we set up the
# breakpoints on all known functions.
#
# This is because when an Entry Point Address is hit, all
# dependent shared/dynamic libraries should have been
# loaded by ld.so, and thus we  can set breakpoints
# on functions inside these shared/dynamic libraries without
# getting GDB "memory access" errors.

cat > $TRACE <<EOF
    set width 0
    set height 0
    set pagination off
    set breakpoint pending on
    set backtrace past-main on
    set backtrace past-entry on
    set disable-randomization on
    set demangle-style none
    set print demangle off
    set verbose off
    set confirm off
EOF

if [ 0 == $staticBin ]; then
  echo $entryAddr >> $TRACE
  cat >> $TRACE <<EOF
  tbreak main
  tbreak _start
  tbreak _libc_start_main
  run
  delete
EOF
fi

gdb --nx --batch --command=$GETFUNCS $FILE 2>/dev/null | cut -d' ' -f 1 | sort | uniq | awk '

/0x[0-9a-z]+/ {
  printf("break *%s\n", $1);
  print "commands";
  print "bt 2";
  print "continue";
  print "end";
  print "";
}

' >> $TRACE


cat >> $TRACE <<EOF
  catch syscall
  commands
  bt 2
  continue
  end
EOF


if [ 0 == $staticBin ]; then
  echo "continue" >> $TRACE
else
  echo "run" >> $TRACE
fi

# Now run the $TRACE GDB script and collect the
# call trace
gdb --nx --batch --command=$TRACE --tty=/dev/null --args $FILE $@ 2>/dev/null | awk '
BEGIN {
  isFuncCall = 0;
  isSysCall = 0;
  callee = "";
  caller = "*INITIAL*";
  params = "";
}

/^Breakpoint [0-9]+,[ \t]+0x/ {
  isFuncCall = 1;

  callee = $5;
  params = "";
}

/^Breakpoint [0-9]+,[ \t]+[_a-zA-Z]/ {
  isFuncCall = 1;

  callee = $3;
  params = "";
}

/^Catchpoint [0-9]+/ {
  isSysCall = 1;
  split($6, a, "\047")
  callee = "syscall:" a[2];
  params = "";
}

/^#1[ \t]+0x[0-9a-f]+/ {
  if (isFuncCall) {
    if ("??" != $4)
      caller = $4;
    else
      caller = $2;
  }
}

/^#0[ \t]+0x[0-9a-f]+/ {
  if (isSysCall) {
    if ("??" != $4)
      caller = $4;
    else
      caller = $2;
  }
}

/^$/ {
  if (("" != caller) && ("" != callee) && (caller != "*INITIAL*")) {
    printf("%s %s %s\n", caller, callee, params);
    callee = caller = params = "";
    isFuncCall = isSysCall = 0;
  }
}
' | uniq

cat $TRACE