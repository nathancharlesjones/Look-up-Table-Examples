# Other Sin Improvements

## Contents

1. [What is it?](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/README.md#what-is-it)
2. [What's it telling me?](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/README.md#whats-it-telling-me)
3. [How do I use it?](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/README.md#how-do-i-use-it)
4. [How does it work?](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/README.md#how-does-it-work)

## What is it?

"Other Sin Improvements" builds on the demonstration in "01_Fast-Sin" of using a simple look-up table (LUT) to improve the execution time of the library sin function. These LUT examples add different data types, linear interpolation, and arbitrary-input tables in order to improve the execution time, accuracy, or memory size of the LUT in "01_Fast-Sin". It also profiles and compares to the LUT implementations four different implementations of "sin" that utilize various polynomial approximations. Lastly, it builds upon the the professional quality code from "01_Fast-Sin" by parameterizing the test code (to more easily change which functions are being tested).

The following table summarizes the results, though it should be noted that all of the LUTs could be made larger/smaller and more/less accurate by simply increasing or decreasing the number of elements in the table (though care should be taken to ensure that the resulting size allows for a quick and simple hash function, as execution time may be negatively affected if the hash becomes non-trivial). This code was compiled for an STM32F1 running at 72 MHz using GCC 6.3.1 on Ubuntu (there’s also code to run this same example on an x86 so you don’t need an STM32F1 in order to test it, though the results are less drastic). The rest of this README should explain each part of the table, so don't fret if parts of it don't make sense at this time.
|Function|Memory usage (bytes)|Absolute Error|Percent Error|Execution time|
|---|---|---|---|---|
|Library sin|5728|N/A|N/A|\~45-46 us|
|LUT double<sup>2</sup>|3484<sup>1</sup>|\~0.0077<sup>1</sup>|\~1.4<sup>1</sup>|\~6.5 us|
|LUT float<sup>3</sup>|2736<sup>1</sup>|\~0.0076<sup>1</sup>|\~1.4<sup>1</sup>|\~6.2 us|
|LUT fixed<sup>4</sup>|1792<sup>1</sup>|\~0.016<sup>1</sup>|\~4.1<sup>1</sup>|\~0.681 us|
|Dbl interp<sup>5</sup>|3548<sup>1</sup>|\~0.00003<sup>1</sup>|\~0.002<sup>1</sup>|\~13.8 us|
|Flt interp<sup>6</sup>|2848<sup>1</sup>|\~0.00003<sup>1</sup>|\~0.002<sup>1</sup>|\~9.6 us|
|Fxd interp<sup>7</sup>|1808<sup>1</sup>|\~0.00005<sup>1</sup>|\~0.008<sup>1</sup>|\~0.763 us|
|Dbl Non-Uni<sup>8</sup>|1088<sup>1</sup>|\~0.007<sup>1</sup>|\~0.623<sup>1</sup>|\~41.2 us|
|Flt Non-Uni<sup>9</sup>|1424<sup>1</sup>|\~0.007<sup>1</sup>|\~0.636<sup>1</sup>|\~25.5 us|
|Fxd Non-Uni<sup>10</sup>|440<sup>1</sup>|\~0.007<sup>1</sup>|\~0.610<sup>1</sup>|\~3.6 us|
|Sin_32<sup>11</sup>|2408|\~0.006|\~0.438|\~19.8 us|
|Sin_52<sup>12</sup>|2424|\~0.000007|\~0.0016|\~22.2 us|
|Sin_73<sup>13</sup>|1552|\~0.00000005|\~0.00002|\~28.3 us|
|Sin_121<sup>14</sup>|1624|\~0.0000000000007|\~0.0000000006|\~35.3 us|

### Notes:
1. This LUT could be made larger/smaller and more/less accurate by simply increasing or decreasing the number of elements in the table (though care should be taken to ensure that the resulting size allows for a quick and simple hash function, as execution time may be negatively affected if the hash becomes non-trivial).
2. This function is a LUT of doubles with uniform distribution which uses no interpolation. It was stored in RAM in my tests.
3. This function is a LUT of floats with uniform distribution which uses no interpolation. It was stored in RAM in my tests.
4. This function is a LUT of fixed-point numbers (in q15_16 format) with uniform distribution which uses no interpolation. It was stored in RAM in my tests.
5. This function is a LUT of doubles with uniform distribution which uses linear interpolation. It was stored in RAM in my tests.
6. This function is a LUT of floats with uniform distribution which uses linear interpolation. It was stored in RAM in my tests.
7. This function is a LUT of fixed-point numbers with uniform distribution which uses linear interpolation. It was stored in RAM in my tests.
8. This function is a LUT of doubles with non-uniform distribution which uses linear interpolation. It was stored in ROM in my tests.
9. This function is a LUT of floats with non-uniform distribution which uses linear interpolation. It was stored in ROM in my tests.
10. This function is a LUT of fixed-point numbers with non-uniform distribution which uses linear interpolation. It was stored in ROM in my tests.
11. This function is a polynomial approximation of sin which uses 3 terms.
12. This function is a polynomial approximation of sin which uses 4 terms.
13. This function is a polynomial approximation of sin which uses 5 terms.
14. This function is a polynomial approximation of sin which uses 7 terms.

## What's it telling me?

- LUTs can greatly improve both execution time (up to 68 times faster in my tests with the "sin" function) and memory space (up to 13 times smaller in my tests with the "sin" function) at the cost of accuracy (and it's not really possible to have all three).
- To improve a LUT's execution time:
    - Use data types that take advantage of whatever processor the code is running on (e.g. use fixed-point numbers on a processor without a floating-point unit, use 32-bit data types for 32-bit processors, etc).
    - Simplify and/or remove both the hash (the part of the code that converts the input into an appropriate array index) and any post-processing (such as linear interpolation); revert from a non-uniform distribution of x-values to a uniform distribution. (If accuracy is a concern, you could add more points to your LUT.)
- To improve a LUT's accuracy:
    - Add linear interpolation of points (or use a non-uniform distribution of x-values).
    - Add more points to the LUT.
    - Move to higher precision data types (i.e. from fixed-point number to floats to doubles).
- To improve a LUT's memory space:
    - Change to a smaller data type (i.e. from doubles to floats or fixed-point numbers).
    - Use a non-uniform distribution of x-values.
    - Remove points from the LUT (keep conversions in mind (i.e. powers-of-two multiplication); speed reductions could be significant if they aren't).

## How do I use it?

### On my laptop

1. After downloading the Git repo, navigate to the folder "02_Other-Sin-Improvements".
2. Decide whether you want a "debug" or "release" build (the only difference being which optimization flag is passed to the compiler: "-Og" in the case of a "debug" build and "-O2" in the case of a release build) and type `make BUILD={debug or release} TARGET=x86` into a shell (replacing "{debug or release}" with either "debug" or "release", of course). E.g.:
```
$ make BUILD=debug TARGET=x86
```
3. The executable is called "{debug or release}\_x86.elf" and it will be placed in a folder called "build/{debug or release}\_x86".
4. Run the executable; the results of the code will be printed to the terminal. E.g.:
```
$ ./build/debug_x86/debug.elf 
-----Sin LUT Test-----
Number of iterations: 1000

Function        Avg Exec Time (ns)  Max Abs Err     Avg Abs Err     Avg Pcnt Err
------------------------------------------------------------------------------------
Scaffolding     1371.235000         0.999999638702  0.640707775469  100.000000000000
Library Sin     1481.904000         0.000000000000  0.000000000000  0.000000000000
LUT Double      821.099000          0.007646999009  0.002570968708  1.373448721814
LUT Float       484.793000          0.007772471133  0.002441808891  1.987678636571
LUT Fxd Pt      494.717000          0.015221477880  0.005042728490  3.515460252654
Dbl Interp      492.520000          0.000030480362  0.000013038884  0.002098965585
Flt Interp      614.250000          0.000030479392  0.000012803016  0.002005332215
Fxd Interp      498.241000          0.000058739948  0.000021661058  0.007221898182
Dbl X/Y list    521.871000          0.006942957284  0.003539295714  0.620672106137
Flt X/Y list    515.202000          0.006942141193  0.003688976363  0.648580104188
Fxd X/Y list    512.217000          0.006962308063  0.003662528162  0.642273415988
Sin_32          498.588000          0.000596877825  0.000380734589  0.186339542740
Sin_52          499.859000          0.000007003823  0.000004232056  0.002679423559
Sin_73          516.568000          0.000000046535  0.000000029175  0.000012717415
Sin_121         474.864000          0.000000000001  0.000000000000  0.000000000235
```
5. Run `make BUILD={debug or release} TARGET=x86 clean` to remove the "build/{debug or release}\_x86" folder. E.g.:
```
$ make BUILD=debug TARGET=x86 clean
```

### On an STM32

1. Procure an STM32F103C8T6, sometimes called a "Blue Pill".
2. After downloading the Git repo, navigate to the folder "02_Other-Sin-Improvements".
2. Decide whether you want a "debug" or "release" build (the only difference which optimization flag is passed to the compiler: "-Og" in the case of a "debug" build and "-O2" in the case of a release build) and type `make BUILD={debug or release} TARGET=STM32F1` into a shell (replacing "{debug or release}" with either "debug" or "release", of course). E.g.:
```
$ make BUILD=debug TARGET=STM32F1
```
3. The executable is called "{debug or release}\_STM32F1.elf" and it will be placed in a folder called "build/{debug or release}\_STM32F1".
4. Connect your STM32F1 to your computer using your debugger of choice (e.g. ST-Link, J-Link, etc) and start a GDB server. If you have previously downloaded STM32CubeIDE, you can start a GDB server from the command line using the script "Start-ST-Link-GDB-Server_v2.sh" (after updating the file paths appropriately). E.g.:
```
$ ./Start-ST-Link-GDB-Server_v2.sh
```
5. Start GDB and then connect to your STM32. E.g.:
```
$ arm-none-eabi-gdb build/debug_STM32F1/debug.elf --tui
(gdb) target remote :61234
```
6. Load and run the executable. E.g.:
```
(gdb) load build/debug_STM32F1/debug.elf
(gdb) c
```
7. The debugger will halt inside the function `printResults_CUT`. You'll be able to view the same parameters as above by printing each array element from GDB. E.g.: 
```
(gdb) c
Continuing.

Program received signal SIGTRAP, Trace/breakpoint trap.
0x08004112 in printResults_CUT (iterations=1000, codeUnderTest=0x20004b18, 
    codeUnderTest@entry=0x20004b20) at hardware/source/STM32F1/source/STM32F1.c:296
296         HALT_IF_DEBUGGING();
(gdb) p codeUnderTest[1]
$1 = {fcn_name = 0x8004c1c "Library Sin", function_enum = fcn_dbl_in_dbl_out, {
    fcn_double = 0x80001b5 <sin>, fcn_float = 0x80001b5 <sin>, 
    fcn_fixedPoint = 0x80001b5 <sin>}, executionTime_ns = 64903853, 
  executionTime_ns_avg = 64903.853000000003, absoluteError_sum = 0, absoluteError_avg = 0, 
  absoluteError_max = 0, percentError_sum = 0, percentError_avg = 0}
(gdb) p codeUnderTest[6]
$2 = {fcn_name = 0x8004c58 "Flt Interp", function_enum = fcn_flt_in_flt_out, {
    fcn_double = 0x800370d <sin_LUT_float_interpolate>, 
    fcn_float = 0x800370d <sin_LUT_float_interpolate>, 
    fcn_fixedPoint = 0x800370d <sin_LUT_float_interpolate>}, executionTime_ns = 27818984, 
  executionTime_ns_avg = 27818.984, absoluteError_sum = 0.012910090567522466, 
  absoluteError_avg = 1.2910090567522466e-05, absoluteError_max = 3.0493971900802386e-05, 
  percentError_sum = 2.093253463428784, percentError_avg = 0.0020932534634287838}
(gdb)
```

For more information about how the Makefile works, see [here](https://github.com/nathancharlesjones/Generic-Makefile-based-Project-for-x86-and-STM32F1).

## How does it work?

For the sake of brevity, I'll no reiterate what was discussed in the "How does it work?" section of "01_Fast-Sin" (which includes ******)

### sin_lut.c

Let's start with `sin_lut.c`, the heart (if not the meat) of this little example. In lines 8-77, we define an array of doubles called `sin_table`.
```
#define SIN_LUT_SIZE 402
static const double sin_table[SIN_LUT_SIZE] =
{
     0.00000000000000000,  0.01562436422488340,  0.03124491398532610,  0.04685783574813420,  0.06245931784238020,  0.07804555138996730,
     0.09361273123551290,  0.10915705687532200,  0.12467473338522800,  0.14016197234706400,  0.15561499277355600,  0.17103002203139500,
     // Many more rows
    -0.18830433415331500, -0.17293649256293000, -0.15752643100814300, -0.14207791163447200, -0.12659470597635800, -0.11108059403640400,
    -0.09553936336253460, -0.07997480812332720, -0.06439072818171630, -0.04879092816731250, -0.03317921654755680, -0.01755940469793780
};
```
At each index X is stored the value of sin(X/64), allowing us to find the value of any radian angle from zero to 2\*PI simply by accessing the array (e.g. `sin[50]` is equivalent to `sin(50/64)`; notice how the first statement is merely an array access while the second is a library function call). The array size (which may seem odd) was chosen to be a power-of-two multiple of 2\*PI (or as close to it as makes sense) whose value was in the low-hundreds. The "power-of-two multiple" criteria was imposed to make converting from radians to array indices easier (power-of-two multplies or divides are far easier to execute in software than any of type of multiplication or division). The "value in the low-hundreds" criteria was imposed in order to have enough data points to keep the average error low (a table with only 10 elements will have a lot of error for values that lie in between the elements, since the data points are so far apart; however, a table with 1000 elements or more would take up too much space in memory). The array is made `const` so that it is stored in ROM, not RAM; either is fine, but RAM is more often smaller and in higher demand than ROM on an embedded system. The sin_table array is also made `static` and a function is written to access it so that no other code can depend on our implementation of sin_table; it can change in the future and as long as our function still takes a double as input and returns a double that is the sin of the input, then no other code needs to change.
```
double sin_LUT(double radians)
{
	...
    // Not shown: Converting "radians" to an array index and also ensuring that it's a valid index
    ...
	return sin_table[ idx ];
}
```
This function converts the input to an appropriate integer and also makes sure that it falls within the array bounds (see the code comments for additional details).

### main.c

Now let's move to `main.c`. The important stuff happens in lines 48-59 and 63-74.
```
for( int idx = 0; idx < testIterations; idx++ )
{
    double output __attribute__((unused));
    double input = (double) rand() / (double) RAND_MAX * 2.0 * PI;
    err = getSystemTime(p_start);
    ASSERT( err == 0 );
    output = sin( input );
    err = getSystemTime(p_end);
    ASSERT( err == 0 );
    executionTime_sin_ns += systemTimeDiff_ns(p_start, p_end);
}
executionTime_sin_ns_avg = executionTime_sin_ns / testIterations;
```
In those blocks, we're repeatedly creating a random input value, storing the current "system time" into a variable called p_start, calling the function we want to profile, storing the updated system time into a variable called p_end, and computing the difference between the two times (in nanoseconds). That process happens "testIterations" numbers of times and then the sum is divided by "testIterations" to determine the average execution time for the function. The time-related functions are defined elsewhere in the project, meaning that they are "hardware independent", so far as the main function is concerned. They are defined as returning an error value: `0` for success and non-zero for an error. So these blocks also ASSERT that the return value isn't an error code; the ASSERT macro is defined elsewhere (don't worry, we'll get to that too). Following the definition of the `output` variable is a compiler directive, `__attribute__((unused))`, which suppresses warnings about this being an unused variable.

Lines 78-105 compute the error associated with the sin LUT. We're not concerned with the time any of the function calls take so there's no timing code. After computing a random number from zero to 2\*PI, we compute the sin of that value using both the library function and our LUT. The absolute value of the difference between the two results is the absolute error; the absolute error divided by the expected value (the value returned by the library sin function) times 100 is the percent error. This block of code also checks if the expected value is zero and sets the percent error equal to 100 when it is (in order to avoid a deadly division by zero).
```
for( int idx = 0; idx < testIterations; idx++ )
{
    double absoluteError;
    double percentError;

    double rad = (double) rand() / (double) RAND_MAX * 2.0 * PI;
    double output_sin = sin( rad );
    double output_sin_LUT = sin_LUT( rad );
        
    absoluteError = fabs( output_sin - output_sin_LUT );
    absoluteError_sin_LUT_sum += absoluteError;

    double expected = fabs(output_sin);
    if ( expected != 0 ) percentError = absoluteError / expected * 100.0;
    else percentError = 100.0;
    percentError_sin_LUT_sum += percentError;
}
absoluteError_sin_LUT_avg = absoluteError_sin_LUT_sum / testIterations;
percentError_sin_LUT_avg = percentError_sin_LUT_sum / testIterations;
```
Lastly, `printResults` is called, which makes the results of the test available to the developer. It is hardware-specific and will be discussed below.

At the top and bottom of this file are a few pragmas, or compiler directives, that instruct GCC to optimize this file at "O0", the lowest level of optimization, regardless of how the rest of the project is being optimized. This is required so that the compiler doesn't accidentally optimize away our various calls to `sin` and `sin_LUT`, whose outputs are not used and which appear to have no outward effect on the final result of the program.
```
#pragma GCC push_options
#pragma GCC optimize ("O0")
...
#pragma GCC pop_options
```

### hardwareAPI.h

At this point, you have all of the information you need to understand how the program works and what it's doing to compute the comparative execution times and absolute and percent errors. To see how we implement some of those hardware-specific functions, though, we need to look at `hardwareAPI.h` (in hardware/include). This file exposes the application programming interface (API; also simply "interface") that `main.c` can use to complete its actions. Each of the functions and data types not specified here (which are all hardware-specific) will need to be defined elsewhere for whichever processor for which we are compiling the program.

Lines 6-13 define the ASSERT macro and declare a function called `assert_failed`. The ASSERT macro simply checks if `expr` ("expression") is true and, if it isn't, it calls `assert_failed` with the filename and line number where the ASSERT macro was called.
```
void assert_failed(const char * file, uint32_t line);

#define ASSERT(expr)                                \
    do {                                            \
        if (!(expr)) {                              \
            assert_failed(__FILENAME__, __LINE__);  \
        }                                           \
    } while (0)
```

Lines 15 and 16 define two data types: `errno_t` and `p_systemTime_t`. The first is simply a stand-in for an `int` and is used to make it exceedingly clear which functions return the result of an operation and which return an error code. The second represents a pointer to another data type called `systemTime`, which isn't defined here. The hardware implementations will need to define it and the `p_systemTime_t` data type is declared here so that `main.c` can refer to it without needing to know the full size or contents of `systemTime_t` (the C compiler will happily make room for and pass around pointers-to-structs, but if we tried to create a variable that was the struct itself, the C compiler would need to know how big it was, at a minimum, in order to compile our program).
```
typedef int                     errno_t;
typedef struct systemTime_t *   p_systemTime_t;
```

Line 18 declares a function that allows the hardware to initialize anything it needs.

Lines 20-23 declare the timing functions.
- `systemTime_create` creates a `systemTime_t` struct and returns a pointer to it.
- `getSystemTime` puts the current system time, however that's defined, into the variable `thisTime` and returns a `0` on success and non-zero value on failure.
- `elapsedSystemTime_ns` computes the elapsed system time in nanoseconds and stores it in the variable `time_ns`.
- `systemTimeDiff_ns` computes the difference in nanoseconds between two pointers to `systemTime_t` (`p_start` and `p_end`) and returns that difference.

Line 25 declares the `printResults` function.

### x86.c

This file defines all of the hardware-specific stuff that was declared in `hardwareAPI.h` for an x86 processor; this is what allows us to compile and run the program on our laptops.

Lines 11-15 implement the `assert_failed` function, which simply writes a message to stdout (the terminal) and then terminates the program.

Lines 17-20 defines the `systemTime_t` struct as containing another struct called `thisTime`. The data type `struct timespec` is used by `time.h` to store system times and has two data members: `tv_sec` (which holds a value for seconds) and `tv_nsec` (which holds a value for nanoseconds). For a variable of type `p_systemTime_t` named `p_mySystemTime` (the data type that `main` will use), we can access `thisTime` using `mySystemTime->thisTime` and we can access `tv_sec` and `tv_nsec` using `mySystemTime->thisTime.tv_sec` and `mySystemTime->thisTime.tv_nsec`, respectively.
```
struct systemTime_t
{
    struct timespec thisTime;
};
```

Lines 22-26 define the `initHardware` function; there's nothing to initialize for x86 so this function is empty (except for a necessary return statement).

Lines 28-72 define the timing functions.
- `systemTime_create` calls `calloc` to create a pointer to an instance of `systemTime_t`.
- `getSystemTime` uses the function `clock_gettime` from `time.h` to store the system time in the `struct timespec` part of `systemTime_t`.
```
errno_t getSystemTime(p_systemTime_t p_thisTime)
{
    errno_t err = 0;

    ASSERT(p_thisTime != NULL);

    err = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &(p_thisTime->thisTime));
    
    return err;
}
```
- `elapsedSystemTime_ns` uses the function `clock_gettime` as above and then accesses its two data members, `tv_sec` and `tv_nsec` to compute the elapsed system time in nanoseconds.
- `systemTimeDiff_ns` uses `tv_sec` and `tv_nsec` from the `struct timespec` part of the two `systemTime_t` arguments to compute their difference.
```
uint32_t systemTimeDiff_ns(p_systemTime_t p_startTime, p_systemTime_t p_endTime)
{
    ASSERT(p_startTime != NULL);
    ASSERT(p_endTime != NULL);

    // NOTE: No checks are made for overflow, meaning the results of this function are undefined
    // if the difference in nanoseconds between p_startTime and p_endTime is greater than UINT32_MAX
    // (i.e. greater than 4,294,967,295, or 4.294967295 seconds).
    uint32_t diff_sec = p_endTime->thisTime.tv_sec - p_startTime->thisTime.tv_sec;
    uint32_t diff_ns = p_endTime->thisTime.tv_nsec - p_startTime->thisTime.tv_nsec;
    
    return ( ( 1000000000 * diff_sec ) + diff_ns );
}
```

Lastly, `printResults` is defined as using `printf` to display the results of the test.

### STM32F1.c

This file defines all of the hardware-specific stuff that was declared in `hardwareAPI.h` for an STM32 processor; this is what allows us to compile and run the program on our "Blue Pills". Running our program on a real STM32 requires a few additional files (you can see them in the hardware/source/STM32F1 folder) which I obtained by first creating a project in STM32CubeMX with my desired settings (72 MHz CPU frequency, GPIO output on the build-in LED, etc) and setting the project type as "Makefile" before clicking "Generate code". The only files I changed from the project generated by STM32CubeMX were to their `main.c` and `main.h` files which, among other things, were renamed to `STM32F1.c` and `STM32F1.h` (to avoid conflicts with similarly named files in our current project).

Lines 33-37 define the `systemTime_t` struct as containing two 32-bit unsigned integers called `ms` and `ns`. Since the STM32 HAL configures an on-board timer to generate 1 millisecond interrupts, we can use the millisecond counter and the timer value to determine the system time (since start-up) to with approximately nanosecond accuracy.
```
struct systemTime_t
{
    uint32_t ms;
    uint32_t ns;
};
```

On line 71, the signature for `main` is redefined to match the signature for `initHardware`. The `while(1)` loop is also deleted and a return statement is added. I also added a call to `SystemCoreClockUpdate` on line 107, since the variable `SystemCoreClock` is used later on to determine the CPU frequency (see the comments for more information).

Lines 210-222 define the `assert_failed` function. In this case, all it does is turn on the built-in LED (as a warning light of sorts) and enter an infinite loop; if a debugger is attached, then the process halts (`HALT_IF_DEBUGGING` is a macro that's defined in `STM32F1.h`, which uses a special register on Cortex-M processors and an assembly instruction to determine if a debugger is connected and, if it is, to trigger a breakpoint).

Lines 224-290 define the timing functions. They are (essentially) identical to the functions that were defined for the x86 with the exception of how we get the current "system time". As mentioned above, the STM32 HAL implements a millisecond counter using the SysTick timer; `HAL_GetTick()` returns the number of milliseconds that have transpired since the STM32 powered-on and the `SysTick` struct contains the information for the `SysTick` timer. `SysTick->LOAD` is the reload value for the timer and `SysTick->VAL` is the timer's current value; their difference is equal to the number of timer "ticks" since the last millisecond rollover. Dividing this number of "ticks" by the CPU's "ticks per second" (a.k.a. it's CPU frequency) results in the number of seconds that have elapsed since the last rollover; multiplying the number of seconds by 1 billion results in the number of nanoseconds since the last millisecond rollover. The calls to `HAL_GetTick()` and `SysTick->VAL` are placed within a `do...while` loop to catch the infrequent error where the SysTick interrupt triggers in between the two calls; if this were to occur without us checking, then it would be possible for the timer to rollover without us getting the updated value from `HAL_GetTick`.
```
errno_t getSystemTime(p_systemTime_t p_thisTime)
{
    uint32_t SysTick_Value;

    ASSERT(p_thisTime != NULL);

    // Ensure uwTick (accessed through HAL_GetTick) isn't updated while retrieving SysTick->VAL
    //
    do
    {
        p_thisTime->ms = HAL_GetTick();
        SysTick_Value = SysTick->VAL;
    } while( p_thisTime->ms != HAL_GetTick() );

    // NOTE: Order of operations, currently enforced with parentheses, is CRITICAL to prevent
    // overflow.
    //
    p_thisTime->ns = ( ( SysTick->LOAD - SysTick_Value ) * ( 1000000000.0 / (float)SystemCoreClock ) );

    return 0;
}
```

Lines 292-298 define the `printResults` function. Although we could have used any number of peripherals to display this information to the developer (the UART peripheral, in particular), I opted to keep the hardware requirements and development complexity to a minimum and implemented none of them. The information is easily viewable with a debugger, which is exactly how I anticipated this program being run anyhow.