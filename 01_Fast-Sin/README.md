# Fast Sin

## What is it?
"Fast Sin" is a demonstration of using a simple look-up table (LUT) to improve the execution time of the library sin function. It also demonstrates several aspects of professional quality code, such as using asserts, enforcing various compiler optimizations, building for multiple targets, and having a well-structured Makefile.

The following table summarizes the results. This code was compiled for an STM32F1 running at 72 MHz using GCC 6.3.1 on Ubuntu (there’s also code to run this same example on an x86 so you don’t need an STM32F1 in order to test it, though the results are less drastic).
|Function|Memory usage (bytes)|Absolute Error|Percent Error|Execution time|
|---|---|---|---|---|
|sin|5368|N/A|N/A|~63-64 us|
|sin_LUT|2936|0.0028|1.8-2.0|~20.4 us|

## How do I use it?

### On my laptop
1. After downloading the Git repo, navigate to the folder "01_Fast-Sin".
2. Decide whether you want a "debug" or "release" build (the only difference which optimization flag is passed to the compiler) and type "make BUILD={debug or release} TARGET=x86" into a shell (replacing "{debug or release}" with either "debug" or "release", of course).
`make BUILD=debug TARGET=x86`
3. The executable is called "{debug or release}_x86.elf" and it will be placed in a folder called "build/{debug or release}_x86"
4. Run the executable; the results of the code will be printed to the terminal.
```
$ ./build/debug_x86/debug.elf 
-----Sin LUT Test-----
Number of iterations: 1000
Function	Avg Execution Time (ns)	Avg Absolute Error	Avg Percent Error
sin		1562.535034		N/A			N/A
sin_LUT		1380.771973		0.002858		2.041038
```
5. Run "make BUILD={debug or release} TARGET=x86 clean" to remove the "build/{debug or release}_x86" folder.
`make BUILD=debug TARGET=x86 clean`

### On an STM32
1. Procure an STM32F103C8T6, sometimes called a "Blue Pill".
2. After downloading the Git repo, navigate to the folder "01_Fast-Sin".
2. Decide whether you want a "debug" or "release" build (the only difference which optimization flag is passed to the compiler) and type "make BUILD={debug or release} TARGET=STM32F1" into a shell (replacing "{debug or release}" with either "debug" or "release", of course).
`make BUILD=debug TARGET=STM32F1`
3. The executable is called "{debug or release}_STM32F1.elf" and it will be placed in a folder called "build/{debug or release}_STM32F1"
4. Connect your STM32F1 to your computer using your debugger of choice (e.g. ST-Link, J-Link, etc) and start a GDB server. If you have previously downloaded STM32CubeIDE, you can start a GDB server from the command line using the script "Start-ST-Link-GDB-Server_v2" (after updating the file paths appropriately).
`./Start-ST-Link-GDB-Server_v2.sh`
5. Start GDB and then connect to your STM32.
```
$ arm-none-eabi-gdb build/debug_STM32F1/debug.elf --tui
(gdb) target remote :61234
```
6. Load and run the executable.
```
(gdb) load build/debug_STM32F1/debug.elf
(gdb) c
```
7. The debugger will halt inside the function `printResults`. You'll be able to view the same parameters as above in the GDB output, as they are being passed to the function.
```
(gdb) c
Continuing.

Program received signal SIGTRAP, Trace/breakpoint trap.
0x08002b8a in printResults (iterations=iterations@entry=1000,
    executionTime_sin_ns_avg=executionTime_sin_ns_avg@entry=63739.2852,
    executionTime_sin_LUT_ns_avg=executionTime_sin_LUT_ns_avg@entry=20488.1738,
    absoluteError_sin_LUT_avg=0.00271631964, percentError_sin_LUT_avg=2.06115556)
    at hardware/source/STM32F1/source/STM32F1.c:293
```

## How does it work?
