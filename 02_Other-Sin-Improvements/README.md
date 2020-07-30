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
4. Run the executable; the results of the code will be printed to the terminal. "Scaffolding" runs the profiling code with no function call; it informs us how much of the "average execution time" reported for every other function is taken up by the "scaffolding" code, such as storing the start and end times in their respective variables and asserting that there were no errors in doing so. To get the adjusted (i.e. correct) values for each function's execution time, I would think that we should simply be able to subtract off the execution time for "scaffolding", however, doing that with the numbers below doesn't make much sense. My best guess is that my laptop is performing optimizations as the code is running so that functions further down on the list execute faster than earlier ones. E.g.:
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
7. The debugger will halt inside the function `printResults_CUT`. You'll be able to view the same parameters as above by printing each array element from GDB (i.e. `p codeUnderTest[0]` prints the results of the first test). "Scaffolding" (`codeUnderTest[0]`) runs the profiling code with no function call; it informs us how much of the "average execution time" reported for every other function is taken up by the "scaffolding" code, such as storing the start and end times in their respective variables and asserting that there were no errors in doing so. To get the adjusted (i.e. correct) values for each function's execution time, simply subtract off the execution time for "scaffolding". E.g.: 
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

For the sake of brevity, I'll not reiterate what was discussed in the ["How does it work?"](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/01_Fast-Sin/README.md#how-does-it-work) section of "01_Fast-Sin" (which includes how to build a basic LUT; how to test for the average execution times, average absolute errors, and average percent error; and how to structure the project so that it can be compiled and run on both an x86 and STM32F1 processor).

### Parameterizing the tests

Adding tests to or subtracting tests from the code in "01_Fast-Sin" was already getting tedious, and with many more functions to add, I decided to first refactor `main` to make that easier. I wanted to make two major changes: (1) Put the variables that hold the profiling information for each function (e.g. absoluteError_avg, executionTime_ns_sum, etc) into a struct for each function being tested (so that I don't need to keep changing the input parameters to `printResults` every time I change which functions are being tested) and (2) Put a pointer to the functions being tested into that struct so that the profiling code (which doesn't change) can be put in one spot and simply called multiple times with function pointers for each of the functions being tested.

To accomplish #1, I created a struct in `main.h` called `sinLUT_implementation_t` that included a `const char *` for the function name and several variables of type `double` to hold the various pieces of profiling information. Eventually, I could create different instances of `sinLUT_implementation_t` for each function being tested and then simply pass this array to `printResults` (which I renamed `printResults_CUT`). I used an empty struct (`{0}`) to signal the end of the array.
```
typedef struct sinLUT_implementation_t
{
    const char * fcn_name;
    ...
    double executionTime_ns;
    double executionTime_ns_avg;
    double absoluteError_sum;
    double absoluteError_avg;
    double absoluteError_max;
    double percentError_sum;
    double percentError_avg;
} sinLUT_implementation_t;
```
On the surface, accomplishing #2 wasn't awful either. However, the slightly complex part was getting the profiling code to work with different function pointers (that is, pointers to functions with different signatures). To start, I created three function typedefs in `sin_lut.h` to represent my three main functions: `p_sin_LUT_double`, `p_sin_LUT_float`, and `p_sin_LUT_fixedPoint`, corresponding to each function's input/output data types (I did not experiment with mixed data types, such as receiving a fixed-point input and returning a double).
```
typedef double (*p_sin_LUT_double)(double);
typedef float (*p_sin_LUT_float)(float);
typedef q15_16_t (*p_sin_LUT_fixedPoint)(q15_16_t);
```
I then added a function pointer to my new struct `sinLUT_implementation_t`, above. Since each function pointer is, effectively, a different data type, I put the three of them into a `union`. In this manner, the `sinLUT_implementation_t` struct would only store one of them at a time, and an enumerated data type called `fcnSignature_t` (which I also added) would indicate which (if any) of the function pointers was active (for more about unions, see [here](https://www.tutorialspoint.com/cprogramming/c_unions.htm), [here](https://www.geeksforgeeks.org/union-c/), or [here](https://github.com/nathancharlesjones/Flexible-message-format)).
```
typedef enum fcnSignature_t
{
    NOT_ASSIGNED,
    fcn_scaffolding,
    fcn_dbl_in_dbl_out,
    fcn_flt_in_flt_out,
    fcn_fxd_in_fxd_out
} fcnSignature_t;

typedef struct sinLUT_implementation_t
{
    ...
    fcnSignature_t function_enum;
    union
    {
        p_sin_LUT_double fcn_double;
        p_sin_LUT_float fcn_float;
        p_sin_LUT_fixedPoint fcn_fixedPoint;
    };
    ...
} sinLUT_implementation_t;
```
In `main`, now, we start by initializing the array of functions to test. Each function gets a name, a function type, a pointer to the function to call, and zeros for the profiling data.
```
sinLUT_implementation_t codeUnderTest[] = 
{
    { "Scaffolding",    fcn_scaffolding,    {.fcn_double = NULL},                               0, 0, 0, 0, 0, 0, 0 },
    { "Library Sin",    fcn_dbl_in_dbl_out, {.fcn_double = sin},                                0, 0, 0, 0, 0, 0, 0 },
    { "LUT Double",     fcn_dbl_in_dbl_out, {.fcn_double = sin_LUT_double},                     0, 0, 0, 0, 0, 0, 0 },
    { "LUT Float",      fcn_flt_in_flt_out, {.fcn_float = sin_LUT_float},                       0, 0, 0, 0, 0, 0, 0 },
    { "LUT Fxd Pt",     fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint},             0, 0, 0, 0, 0, 0, 0 },
    { "Dbl Interp",     fcn_dbl_in_dbl_out, {.fcn_double = sin_LUT_double_interpolate},         0, 0, 0, 0, 0, 0, 0 },
    { "Flt Interp",     fcn_flt_in_flt_out, {.fcn_float = sin_LUT_float_interpolate},           0, 0, 0, 0, 0, 0, 0 },
    { "Fxd Interp",     fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint_interpolate}, 0, 0, 0, 0, 0, 0, 0 },
    { "Dbl X/Y list",   fcn_dbl_in_dbl_out, {.fcn_double = sin_LUT_double_nonUniform},          0, 0, 0, 0, 0, 0, 0 },
    { "Flt X/Y list",   fcn_flt_in_flt_out, {.fcn_float = sin_LUT_float_nonUniform},            0, 0, 0, 0, 0, 0, 0 },
    { "Fxd X/Y list",   fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint_nonUniform},  0, 0, 0, 0, 0, 0, 0 },
    { "Sin_32\t",       fcn_flt_in_flt_out, {.fcn_float = sin_32},                              0, 0, 0, 0, 0, 0, 0 },
    { "Sin_52\t",       fcn_flt_in_flt_out, {.fcn_float = sin_52},                              0, 0, 0, 0, 0, 0, 0 },
    { "Sin_73\t",       fcn_dbl_in_dbl_out, {.fcn_double = sin_73},                             0, 0, 0, 0, 0, 0, 0 },
    { "Sin_121\t",      fcn_dbl_in_dbl_out, {.fcn_double = sin_121},                            0, 0, 0, 0, 0, 0, 0 },
    {0}
};
```
The profiling code was then restructured to be a `while` loop that iterates over the `codeUnderTest` array until the `NULL` struct is found. For each function, the profiling code runs `testIterations` numbers of loops, each time creating a new random input value, calling the function under test (while profiling it), and computing the errors. After all iterations, the average values are computed, the index variable is incremented, and the `while` loop continues. A `switch...case` statement in the body of the `for` loop decides which part of the union to access, based on which part is active (which is indicated by the `function_enum` variable in the `sinLUT_implementation_t` struct).

Adding new functions is now fairly trivial (provided they match one of the three function signatures used for the function pointers): Simply write the function and then add an element to the `codeUnderTest` array with the appropriate information and the test will automatically get run. Removing a test is as simple as commenting out the line of code that puts it in the array.

### Adding floats

Changing the data type from doubles to floats involved no code changes, except to change `double` to `float`. The LUT could even stay the same, just being stored as floats instead of doubles, since the compiler would handle the actual conversion process. As expected, this resulted in a LUT that was half the size of one that used doubles. In each case tested, the resulting code had roughly the same accuracy as a LUT of doubles though the code ran anywhere from 4.7% to 38% faster.

### Adding fixed-point numbers

What's a "fixed-point" number, you ask? Excellent question! Consider, first, an integer: the least significant bit of an integer has the value "1" (or 2<sup>0</sup>), so 0b0001 is equal to "1" and 0b1001 is equal to "9". There are no digits to the right of the lest significant bit, but what if there were? What would 0b0001.00 mean? If we extend the idea that the third bit of a binary number is 2<sup>3</sup>, the second bit is 2<sup>2</sup>, the first bit is 2<sup>1</sup>, and the zeroth bit is 2<sup>0</sup>, then the bit after the decimal (or "radix") point might be 2<sup>-1</sup> (or 0.5), then 2<sup>-2</sup> (or 0.25), and so on. The digits to the left of the radix point become the "integer" bits and those to the right become the "fractional" bits. This is the meaning of a "fixed-point" number: an integer data type whose radix point is redefined to be somewhere other than to the right of the least significant bit. Assuming our example above of a fixed-point number is signed, "0b0001.00", is said to be in "q3.2" format, meaning that there are 3 integer bits (plus an assumed sign bit) and 2 fractional bits. Instead of representing the range [-32, 31], as it would if it were a plain signed integer, our example can now represent the range [-8, 7.75], in increments of 0.25. Since I am using an STM32F1 with a 32-bit processor to demonstrate these LUTs, I assumed a commonly used fixed-point format might be "q15.16", allowing for a range of [-65536, 65535.999984741], in increments of 0.000015259. For a processor without a floating-point unit, integer math (including operating on fixed-point numbers, since they are an "integer" data type, just with a different radix position) goes MUCH faster and doesn't require additional code (as would be needed to perform floating-point math). For more information, see [here](https://developer.arm.com/documentation/dai0033/a/).

I added the code at the above link into my project directly, adding a typedef for the fixed-point data type I wanted to use.
```
typedef int q15_16_t
```


### Adding linear interpolation

### Changing from a uniform to a non-uniform distribution of x-values

### Comparing to the polynomial approximations