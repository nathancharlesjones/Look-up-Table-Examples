# Other Sin Improvements

## Contents

1. [What is it?](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/README.md#what-is-it)
2. [What's it telling me?](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/README.md#whats-it-telling-me)
3. [How do I use it?](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/README.md#how-do-i-use-it)
4. [How does it work?](https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/README.md#how-does-it-work)

## What is it?

"Other Sin Improvements" builds on the demonstration in "01_Fast-Sin" of using a simple look-up table (LUT) to improve the execution time of the library sin function. These LUT examples add different data types, linear interpolation, and arbitrary-input tables in order to improve the execution time, accuracy, or memory size of the LUT in "01_Fast-Sin". It also profiles and compares to the LUT implementations four different implementations of "sin" that utilize various polynomial approximations. Lastly, it builds upon the the professional quality code from "01_Fast-Sin" by parameterizing the test code (to more easily change which functions are being tested).

The following table summarizes the results, though it should be noted that all of the LUTs could be made larger/smaller and more/less accurate by simply increasing or decreasing the number of elements in the table (though care should be taken to ensure that the resulting size allows for a quick and simple hash function, as execution time may be negatively affected if the hash becomes non-trivial). This code was compiled for an STM32F1 running at 72 MHz using GCC 6.3.1 on Ubuntu (there’s also code to run this same example on an x86 so you don’t need an STM32F1 in order to test it, though the results are less drastic). The rest of this README should explain each part of the table, so don't fret if parts of it don't make sense at this time.
|Function|Memory usage<sup>1</sup> (bytes)|Max Absolute Error|Percent Error|Execution time|
|---|---|---|---|---|
|Library sin|5480|N/A|N/A|\~47 us|
|LUT double<sup>3</sup>|3484<sup>2</sup>|\~0.0076<sup>2</sup>|\~2.5<sup>1</sup>|\~6.7 us|
|LUT float<sup>4</sup>|2736<sup>2</sup>|\~0.0076<sup>2</sup>|\~1.5<sup>1</sup>|\~6.4 us|
|LUT fixed<sup>5</sup>|1808<sup>2</sup>|\~0.0077<sup>2</sup>|\~1.6<sup>1</sup>|\~0.73 us|
|LUT fixed (safe)<sup>6</sup>|2060<sup>2</sup>|\~0.0077<sup>2</sup>|\~1.5<sup>1</sup>|\~1.3 us|
|Dbl interp<sup>7</sup>|3548<sup>2</sup>|\~0.00003<sup>2</sup>|\~0.002<sup>1</sup>|\~13.9 us|
|Flt interp<sup>8</sup>|2848<sup>2</sup>|\~0.00003<sup>2</sup>|\~0.002<sup>1</sup>|\~9.9 us|
|Fxd interp<sup>9</sup>|1840<sup>2</sup>|\~0.00003<sup>2</sup>|\~0.002<sup>1</sup>|\~1.0 us|
|Fxd interp (safe)<sup>10</sup>|2284<sup>2</sup>|\~0.00003<sup>2</sup>|\~0.002<sup>1</sup>|\~2.6 us|
|Dbl Non-Uni<sup>11</sup>|824<sup>2</sup>|\~0.007<sup>2</sup>|\~0.62<sup>1</sup>|\~40.9 us|
|Flt Non-Uni<sup>12</sup>|1432<sup>2</sup>|\~0.007<sup>2</sup>|\~0.63<sup>1</sup>|\~25.8 us|
|Fxd Non-Uni<sup>13</sup>|1376<sup>2</sup>|\~0.007<sup>2</sup>|\~0.61<sup>1</sup>|\~6.5 us|
|Fxd Non-Uni (safe)<sup>14</sup>|1916<sup>2</sup>|\~0.007<sup>2</sup>|\~0.61<sup>1</sup>|\~8.8 us|
|Sin_32<sup>15</sup>|2128|\~0.0006|\~0.44|\~19.7 us|
|Sin_52<sup>16</sup>|2144|\~0.000007|\~0.0016|\~22.0 us|
|Sin_73<sup>17</sup>|1280|\~0.00000005|\~0.00002|\~28.1 us|
|Sin_121<sup>18</sup>|1344|\~0.0000000000007|\~0.0000000006|\~35.1 us|

### Notes:
1. Memory usage was measured very non-academically, by observing the difference in the output of the `size` tool with each function included and them removed. In particular, I wasn't sure if certain external library functions such as type conversions were included when I didn't want them to be. It seems possible to glean this information from the map file. I'd love to hear any better suggestions!
2. This LUT could be made larger/smaller and more/less accurate by simply increasing or decreasing the number of elements in the table (though care should be taken to ensure that the resulting size allows for a quick and simple hash function, as execution time may be negatively affected if the hash becomes non-trivial).
3. "LUT double" is a LUT of doubles with uniform distribution which uses no interpolation. It was stored in RAM in my tests.
4. "LUT float" is a LUT of floats with uniform distribution which uses no interpolation. It was stored in RAM in my tests.
5. "LUT fixed" is a LUT of fixed-point numbers (in q0_31 format) with uniform distribution which uses no interpolation. It was stored in RAM in my tests.
6. "LUT fixed (safe)" is a LUT of fixed-point numbers (in q0_31 format) with uniform distribution which uses no interpolation. It includes checks for integer overflow, which "LUT fixed" does not. It was stored in RAM in my tests.
7. "Dbl interp" is a LUT of doubles with uniform distribution which uses linear interpolation. It was stored in RAM in my tests.
8. "Flt interp" is a LUT of floats with uniform distribution which uses linear interpolation. It was stored in RAM in my tests.
9. "Fxd interp" is a LUT of fixed-point numbers (in q0_31 format) with uniform distribution which uses linear interpolation. It was stored in RAM in my tests.
10. "Fxd interp (safe)" is a LUT of fixed-point numbers (in q0_31 format) with uniform distribution which uses linear interpolation. It includes checks for integer overflow, which "Fxd interp" does not. It was stored in RAM in my tests.
11. "Dbl Non-Uni" is a LUT of doubles with non-uniform distribution which uses linear interpolation. It was stored in ROM in my tests.
12. "Flt Non-Uni" is a LUT of floats with non-uniform distribution which uses linear interpolation. It was stored in ROM in my tests.
13. "Fxd Non-Uni" is a LUT of fixed-point numbers (x-vales in q9_22 format and y-values in q0_31 format) with non-uniform distribution which uses linear interpolation. It was stored in ROM in my tests.
14. "Fxd Non-Uni (safe)" is a LUT of fixed-point numbers (x-vales in q9_22 format and y-values in q0_31 format) with non-uniform distribution which uses linear interpolation. It includes checks for integer overflow, which "Fxd Non-Uni" does not. It was stored in ROM in my tests.
15. "Sin_32" is a polynomial approximation of sin which uses 3 terms. For more information, see [here](http://www.ganssle.com/approx.htm).
16. "Sin_52" is a polynomial approximation of sin which uses 4 terms. For more information, see [here](http://www.ganssle.com/approx.htm).
17. "Sin_73" is a polynomial approximation of sin which uses 5 terms. For more information, see [here](http://www.ganssle.com/approx.htm).
18. "Sin_121" is a polynomial approximation of sin which uses 7 terms. For more information, see [here](http://www.ganssle.com/approx.htm).

## What's it telling me?

- LUTs can greatly improve both execution time (up to 64 times faster in my tests with the "sin" function) and memory space (down to nearly 1/7 the size of the "sin" function in my tests) at the cost of accuracy (and it's not really possible to have all three).
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
    - Remove points from the LUT (keep in mind whether or not the resulting hash is a power-of-two multiplication/division; speed reductions could be significant if it isn't).

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
Scaffolding     1358.093000         0.999986277632  0.644043266789  100.000000000000
Library Sin     1398.741000         0.000000000000  0.000000000000  0.000000000000
LUT Double      803.900000          0.007771433598  0.002471737935  1.530016318615
LUT Float       1351.130000         0.007735169056  0.002420661792  1.596855263944
LUT Fxd Pt      1072.617000         0.007700121593  0.002477780442  1.574373446547
LUT Fxd Safe    479.525000          0.007740844955  0.002449670419  1.028467817398
Dbl Interp      491.222000          0.000030499500  0.000012950002  0.002069582016
Flt Interp      465.969000          0.000030523387  0.000012964398  0.002011081010
Fxd Interp      545.783000          0.000030502013  0.000013326361  0.002086139948
Fxd Intp Sf     472.011000          0.000030495386  0.000012607523  0.002036685640
Dbl X/Y list    509.986000          0.006942980536  0.003529767557  0.618582128514
Flt X/Y list    504.590000          0.006942931688  0.003526180508  0.610200772096
Fxd X/Y list    503.980000          0.006942912754  0.003702842732  0.635282341179
Fxd X/Y Safe    552.198000          0.006941854929  0.003671195024  0.650059049099
Sin_32          516.507000          0.000596921552  0.000387763782  0.389704888031
Sin_52          544.302000          0.000007000131  0.000004332903  0.003556309542
Sin_73          559.736000          0.000000046534  0.000000029694  0.000038951759
Sin_121         513.930000          0.000000000001  0.000000000000  0.000000000163

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
(gdb) p codeUnderTest[7]
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

Adding tests to or subtracting tests from the code in "01_Fast-Sin" was already getting tedious, and with many more functions to add, I decided to first refactor `main` to make that easier. I wanted to make two major changes: (1) Put the variables that hold the profiling information for each function (e.g. `absoluteError_avg`, `executionTime_ns_sum`, etc) into a struct for each function being tested (so that I don't need to keep changing the input parameters to `printResults` every time I change which functions are being tested) and (2) Put a pointer to the functions being tested into that struct so that the profiling code (which doesn't change) can be put in one spot and simply called multiple times with function pointers for each of the functions being tested.

To accomplish (1), I created a struct in `main.h` called `sinLUT_implementation_t` that included a `const char *` for the function name and several variables of type `double` to hold the various pieces of profiling information. Eventually, I could create different instances of `sinLUT_implementation_t` for each function being tested and then simply pass this array to `printResults` (which I renamed `printResults_CUT`).
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
On the surface, accomplishing (2) wasn't awful either. However, the slightly complex part was getting the profiling code to work with different function pointers (that is, pointers to functions with different signatures). To start, I created three function typedefs in `sin_lut.h` to represent my three main functions: `p_sin_LUT_double`, `p_sin_LUT_float`, and `p_sin_LUT_fixedPoint`, corresponding to each function's input/output data types (I did not experiment with mixed data types, such as receiving a fixed-point input and returning a double).
```
typedef double (*p_sin_LUT_double)(double);
typedef float (*p_sin_LUT_float)(float);
typedef q0_31_t (*p_sin_LUT_fixedPoint)(q9_22_t);
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
In `main`, now, we start by initializing the array of functions to test. Each function gets a name, a function type, a pointer to the function to call, and zeros for the profiling data. I used an empty struct (`{0}`) to signal the end of the array.
```
sinLUT_implementation_t codeUnderTest[] = 
{
    { "Scaffolding",    fcn_scaffolding,    {.fcn_double = NULL},                                       0, 0, 0, 0, 0, 0, 0 },
    { "Library Sin",    fcn_dbl_in_dbl_out, {.fcn_double = sin},                                        0, 0, 0, 0, 0, 0, 0 },
    { "LUT Double",     fcn_dbl_in_dbl_out, {.fcn_double = sin_LUT_double},                             0, 0, 0, 0, 0, 0, 0 },
    { "LUT Float",      fcn_flt_in_flt_out, {.fcn_float = sin_LUT_float},                               0, 0, 0, 0, 0, 0, 0 },
    { "LUT Fxd Pt",     fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint},                     0, 0, 0, 0, 0, 0, 0 },
    { "LUT Fxd Safe",   fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint_safe},                0, 0, 0, 0, 0, 0, 0 },
    { "Dbl Interp",     fcn_dbl_in_dbl_out, {.fcn_double = sin_LUT_double_interpolate},                 0, 0, 0, 0, 0, 0, 0 },
    { "Flt Interp",     fcn_flt_in_flt_out, {.fcn_float = sin_LUT_float_interpolate},                   0, 0, 0, 0, 0, 0, 0 },
    { "Fxd Interp",     fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint_interpolate},         0, 0, 0, 0, 0, 0, 0 },
    { "Fxd Intp Sf",    fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint_interpolate_safe},    0, 0, 0, 0, 0, 0, 0 },
    { "Dbl X/Y list",   fcn_dbl_in_dbl_out, {.fcn_double = sin_LUT_double_nonUniform},                  0, 0, 0, 0, 0, 0, 0 },
    { "Flt X/Y list",   fcn_flt_in_flt_out, {.fcn_float = sin_LUT_float_nonUniform},                    0, 0, 0, 0, 0, 0, 0 },
    { "Fxd X/Y list",   fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint_nonUniform},          0, 0, 0, 0, 0, 0, 0 },
    { "Fxd X/Y Safe",   fcn_fxd_in_fxd_out, {.fcn_fixedPoint = sin_LUT_fixedPoint_nonUniform_safe},     0, 0, 0, 0, 0, 0, 0 },
    { "Sin_32\t",       fcn_flt_in_flt_out, {.fcn_float = sin_32},                                      0, 0, 0, 0, 0, 0, 0 },
    { "Sin_52\t",       fcn_flt_in_flt_out, {.fcn_float = sin_52},                                      0, 0, 0, 0, 0, 0, 0 },
    { "Sin_73\t",       fcn_dbl_in_dbl_out, {.fcn_double = sin_73},                                     0, 0, 0, 0, 0, 0, 0 },
    { "Sin_121\t",      fcn_dbl_in_dbl_out, {.fcn_double = sin_121},                                    0, 0, 0, 0, 0, 0, 0 },
    {0}
};
```
The profiling code was then restructured to be a `while` loop that iterates over the `codeUnderTest` array until the `NULL` struct is found. For each function, the profiling code runs `testIterations` numbers of loops, each time creating a new random input value, calling the function under test (while profiling it), and computing the errors. After all iterations, the average values are computed, the index variable is incremented, and the `while` loop continues. A `switch...case` statement in the body of the `for` loop decides which part of the union to access, based on which part is active (which is indicated by the `function_enum` variable in the `sinLUT_implementation_t` struct).
```
int idx_CUT = 0;
while( codeUnderTest[idx_CUT].function_enum != NOT_ASSIGNED )
{
    for( int idx_test = 0; idx_test < testIterations; idx_test++ )
    {
        ...
        switch( codeUnderTest[idx_CUT].function_enum )
        {
            case fcn_scaffolding:
                // Run profiling code
            break;

            case fcn_dbl_in_dbl_out:
                // Run profiling code
            break;

            case fcn_flt_in_flt_out:
                // Run profiling code
            break;

            case fcn_fxd_in_fxd_out:
                // Run profiling code
            break;

            default:
                ASSERT(0);
            // Unreachable
            break;
        }   
        ...
    }
    ...
    idx_CUT++;
}
```
Adding new functions is now fairly trivial (provided they match one of the three function signatures used for the function pointers): Simply write the function and then add an element to the `codeUnderTest` array with the appropriate information and the test will automatically get run. Removing a test is as simple as commenting out the line of code that puts it in the array.

### Filling in the LUTs at Runtime

One other small architectural change from "01_Fast-Sin" involved how the LUTs were filled: instead of being explicitly defined with each value of sin (which was getting tedious), I wrote a function called `init_sinLUT` which iterates over each of the arrays (of doubles, floats, and fixed-point numbers) and assigns each array element the value of `sin(index/64)`, converted to the appropriate data type.
```
static double sinTable_double[SIN_LUT_SIZE];
static float sinTable_float[SIN_LUT_SIZE];
static q0_31_t sinTable_fixedPoint[SIN_LUT_SIZE];

void init_sinLUT(void)
{
    for( int idx = 0; idx < SIN_LUT_SIZE; idx++)
    {
        double radians = ( (double) idx ) / 64.0;
        double output_double = sin( radians );
        sinTable_double[idx] = output_double;
        sinTable_float[idx] = (float) output_double;
        sinTable_fixedPoint[idx] = TOFIX(output_double, 31);
    }
}
```
The upside is that this implementation is far more straightforward and doesn't suffer from the risk of a typo causing errant bugs (as a hand-typed array of many hundreds of elements might). The downside is that the table must now be stored in RAM, which is often a more precious resource in embedded systems than is ROM. In our case, we can get away with this because we have ample RAM on both processors (as much as my RAM + SSD can hold on my laptop and 20kB on the STM32F1) and no other part of our project is competing for this space.

### Adding floats

Changing the data type from doubles to floats involved no code changes, except to change `double` to `float`. The LUT could even stay the same, just being stored as floats instead of doubles, since the compiler would handle the actual conversion process. As expected, this resulted in a LUT that was half the size of one that used doubles. In each case tested, the resulting code had roughly the same accuracy as a LUT of doubles though the code ran anywhere from 4.7% to 58% faster.

### Adding fixed-point numbers

What's a "fixed-point" number, you ask? Excellent question! Consider, first, an integer: the least significant bit of an integer has the value "1" (or 2<sup>0</sup>), so 0b0001 is equal to "1" and 0b1001 is equal to "9". There are no digits to the right of the lest significant bit, but what if there were? What would 0b0001.00 mean (notice the decimal point)? If we extend the idea that the third bit of a binary number is 2<sup>3</sup>, the second bit is 2<sup>2</sup>, the first bit is 2<sup>1</sup>, and the zeroth bit is 2<sup>0</sup>, then the bit after the decimal (or "radix") point might be 2<sup>-1</sup> (or 0.5), then 2<sup>-2</sup> (or 0.25), and so on. The digits to the left of the radix point become the "integer" bits and those to the right become the "fractional" bits. This is the meaning of a "fixed-point" number: an integer data type whose radix point is redefined to be somewhere other than to the right of the least significant bit. Assuming our example above of a fixed-point number, "0b0001.00", is a signed integer (in two's-complement), it is said to be in "q3.2" (or "q2") format. This means that there are 3 integer bits (plus an assumed sign bit) and 2 fractional bits. Instead of representing the range [-32, 31], as it would if it were a plain signed integer, our example can now represent the range [-8, 7.75], in increments of 0.25. For a 32-bit processor, such as the STM32F1 we are using in these examples, it makes sense to make all fixed-point numbers 32-bits long and a commonly used format might be "q15.16" (or "q16"), allowing for a range of [-65536, 65535.999984741], in increments of 1.5259e-5 (2<sup>-16</sup>). For a processor without a floating-point unit, integer math (including operating on fixed-point numbers, since they are an integer data type, just with a different radix position) goes MUCH faster and doesn't require additional code (as would be needed to perform floating-point math). For more information, see [here](https://developer.arm.com/documentation/dai0033/a/).

I added the code at the above link into my project directly, adding two typedefs for the fixed-point data types I wanted to use.
```
// Define a fixed-point type with 1 sign bit (implied), 0 integer bits, and 31 fractional bits
typedef int32_t q0_31_t;

// Define a fixed-point type with 1 sign bit (implied), 9 integer bits, and 22 fractional bits
typedef int32_t q9_22_t;
```
The first covers a range from [-1, 0.99999999976] in increments of 2.3283064e-10, perfect for representing the output values of our sin LUTs. The second covers a range from [-512, 511.999999762] in increments of 2.38418579e-7; it's used to represent the input values. Why is the integer portion so large if the input will only be as large as 2\*PI? Because for two of the fixed-point LUTs, the input is multiplied by 64 in order to get the array index and any fixed-point number with less than 9 integer bits could possibly overflow, causing erroneous results.

Since multiplying and dividing fixed-point numbers changes the location of the radix point (see the link above for an explanation why), special operations are needed in order to preserve the position of the radix (or, more accurately, special operations are needed in order for the developer to specify what they want the final position of the radix to be). The code at that link provides these functions for us (such as `FDIV` and `FSUBG`), including a few that may seem redundant but which provide for a consistent interface (such as `FADD` and `FMULI`), as well as a few really useful operations (`TOFLT`, `TOFIX`, and `FCONV`). These functions can (mostly) be used as direct stand-ins for the normal integer or floating-point operations in our LUT functions (i.e. `double x = radians * 64` could be replaced with `q9_22_t x = FMULI(radians, 64)`).

**HOWEVER**, I offer two points of caution when using fixed-point numbers:

1. You are responsible for manually managing the various fixed-point data types you use in your program, which gets harder the more fixed-point data types you use. It was extremely easy while developing these examples for me to write some fixed-point code and only after I encountered a bug to realize that I hadn't updated a single `q` argument which caused the result of a certain function to not come out in the q format that I had thought it would (i.e. I would update a line like `q3_28_t x = FMULG( a, b, 28, 22, 28);` to put `x` into q9.22 format and change it to be `q9_22_t x = FMULG( a, b, 28, 22, 28);`, forgetting that the fifth argument to `FMULG` needs to match the q format of the output; the line should read `q9_22_t x = FMULG( a, b, 28, 22, 22);`, assuming a is in q3.28 and b is in q9.22 format).

2. Integer overflow is MUCH easier to fall into with fixed-point numbers, since you're often sizing them to be only exactly as big as you need them, forgetting that (almost) any math operation might cause these integers to exceed their maximum or minimum values. This happens in two ways:

    1. Forgetting what the maximum value is for a given fixed-point number. As embedded developers, we're used to being able to multiply integers by numbers up to a million or so and not have to worry about overflow. However, it's easy to forget that a number like "1.33..." is actually represented in q3.28 as "357,913,941" and multiplying this number by anything greater than 6 would result in an overflow. We saw an example of this above, in which the input values for our sin LUTs needed to extend up to at least 403 in order to match our array indices, even though the input values should only ever really be as high as 2\*PI.

    2. Not leaving enough "head room" for fixed-point multiplications and divisions. The fixed-point library at the link above performs all multiplication and divisions BEFORE shifting the result up or down to match the final radix, meaning that even though a result might fit into the fixed-point data type to which an operation is being assigned, the operation may still fail if the intermediary result is greater than the maximum integer value (or less than the minimum value). Consider the following multiplication:
        ```
        typedef int8_t q3_4_t;
        // This code produces INCORRECT results
        q3_4_t x = 0b01100000 * 0b00010000; // x equals 6 * 1;
        ```
        Here we're using numbers in q3.4 format, so the first binary value equals "6" and the second "1". Although the result of this operation _should_ be "6", the _actual_ result of the binary multiplication is `0b0000011000000000`. Although this looks nothing like "6" in q3.4, it _does_ look like "6" in q7.8. And this is exactly what happens to the radix during multiplications (and divisions): it gets shifted by the sum (or difference) of the radix positions for the two operands. By shifting the result of our multiplication to the right by the value of our initial radix (i.e. `0b0000011000000000 >> 4`), we get the correct final result: `0b01100000`, or "6". The fixed-point library above performs these shifts automatically.
        ```
        typedef int8_t q3_4_t;
        // This code STILL produces INCORRECT results (see below)
        q3_4_t x = FMUL( 0b01100000, 0b00010000, 4 ); // Equivalent to "( 0b01100000 * 0b00010000 ) >> 4"
        ```
        In fact, however, since we've defined our variables to be signed, 8-bit integers, the result of this multiplication is STILL incorrect. The C language states that the result of a signed integer multiplication which exceeds the maximum value able to be stored by that integer is undefined. Many compilers truncate the answer to the lower bits, as would happen for a multiplication overflow using unsigned integers (but they aren't required to). In our example above, that means that the result of our multiplication _isn't_ `0b0000011000000000` but, more likely, `0b00000000` or "0", which are the lower 8-bits of the resulting multiplication. The simplest way to fix this is to cast the operands to a larger data type BEFORE multiplying them together, so that the upper bits are preserved.
        ```
        typedef int8_t q3_4_t;
        // This code FINALLY produces the CORRECT results
        q3_4_t x = FMUL( (int16_t)0b01100000, (int16_t)0b00010000, 4 );
        ```
        You could also just size the variables larger and make a note to only ever use the lower bits.
        ```
        typedef int16_t q3_4_t; // The fixed-point number is in "q3.4", only 8 bits long, though it will get stored as an in16_t.
        q3_4_t x = FMUL( 0b01100000, 0b00010000, 4 );
        ```

Integer overflow is a commonly exploited security bug (in very similar ways to a "buffer overflow", in which an array is written to or accessed beyond its bounds), so it's my opinion that integer operations should be checked for overflow after (or as a part of) each operation. To do that, I added the `SAFE_` functions to `fixed_point.h`. These functions store the 32-bit operands into 64-bit integers before executing the desired operation (solving problem 2.ii. above of fixed-point numbers being truncated before they are able to be shifted down). The `SAFE_` functions also check for overflow AFTER the shift; if the result is GREATER than INT32_MAX or LESS than INT32_MIN, then no value is stored in the output variable and a `-1` is returned, indicating an error. However, if the safety check passes, then the result of the math operation is stored in the output variable and a value of `0` is returned, indicating "no error". The LUT code that uses these functions (which all end in `_safe`) checks this value to make sure no math operations inadvertently overflowed. I chose to `ASSERT` that the error code returned is `0`, since I figured that anything else constituted a programming error. However, it is possible to recover from a fault like that, and another developer may simply chose to retry the operation with a larger data type for the return variable.

The fixed-point LUTs were significantly smaller and faster than either the double or float LUTs (even after including overflow checks), though at the cost of a fair bit of added complexity.

### Adding linear interpolation

So far our LUTs have done nothing more complicated than map the input value onto the range [0, 403] and perform a simple rounding; this is called a "midpoint interpolation", since the interpolation is defined in segments for which a midpoint is returned as the approximation of the function within that segment. (The midpoints in our LUTs are the array elements and the segments are the range above and below each midpoint or array element which will map back to that midpoint; effectively +/- 0.5 for us.) A depiction of this type of LUT appears below.

<img src="https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/docs/Interpolation_Midpoint.png" width="500">

A look-up table of this type will have a maximum error of <img src="https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/docs/Equation_Error_Midpoint-Interpolation.png" width="150"> (where `h` is the difference in x-values between two elements in the LUT), which, for our sin LUTs, comes out to be 0.0078125 near the zero-crossing points and which approaches 0 near the local maxima and minima (to understand why, see Chapter 2 [here](https://ocw.mit.edu/ans7870/2/2.086/S13/MIT2_086S13_Textbook.pdf)). This matches the results of our profiling above, specifically for the implementations called `LUT double`, `LUT float`, `LUT fixed`, and `LUT fixed (safe)`.

However, if we assume that each pair of adjacent elements in our LUT is connected with a line, we can get a much more accurate answer for input values that fall in-between the LUT elements. We do this by figuring out where on the line the input would fall and returning the resulting y-value. This is called "piecewise linear interpolation" (PwLI) or, simply, "linear interpolation". A depiction of this type of LUT appears below.

<img src="https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/docs/Interpolation_Piecewise-Linear.png" width="500">

A look-up table of this type will have a maximum error of <img src="https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/docs/Equation_Error_Linear-Interpolation.png" width="150"> (with `h` as above), which, for our sin LUTs, approaches 0 near the zero-crossing points and comes out to about 0.000030518 near the local maxima and minima (to understand why, follow the link above). This also matches the results of our profiling above, specifically for the implementations called `Dbl interp`, `Flt interp`, `Fxd interp`, and `Fxd interp (safe)`. The error for a linear interpolation is 0.4% of that for a midpoint interpretation, representing an increase in accuracy of nearly 256 times that for the midpoint interpolation, for the exact same LUT.

The quickest and most straightforward way to perform the linear interpolation is by using the point-slope equation (below), where `x0` and `y0` represent a point on our line and `m` is the slope of the line at that point.
```
y = m * ( x - x0 ) + y0
```
Our LUT is a table of points and by determining the slope between any two of those points, we have all the information we need to compute the linear interpolation. For any input, `x`, we would start by finding the x-values or indices that lay immediately above and below it: `x1` and `x0` (`x0` < `x1`). This could be as simple as assigning the integer portion of our input to `x0` (after multiplying by 64, of course) and assigning that number plus one to `x1`.
```
int x0 = (int)( radians * 64 );
int x1 = x0 + 1:
```
However, care must be taken to ensure that `x0` doesn't equal the last element in the array, since that would cause `x1` to point to garbage data. I've accounted for that in my implementation by sizing my LUT one element larger than it needs to be: 2\*PI\*64 is roughly 402.123859659 so any input, being constrained to 2\*PI, can't exceed this value and the largest number that can get assigned to `x0` is 402 and to `x1`, 403. Thus, for an array that has an element at index 403 (like ours), there is no way for `x1` to point past the array. Another way to account for this is to check if `x0` is equal to the last index and either move it back one or set `x1` to 0 (assuming the function is periodic).

The slope is the difference in y-values over the difference in x-values (which simplifies to just the difference in y-values when the difference in x-values is 1). The "span" is a term I borrowed from [this](https://www.microchip.com/wwwAppNotes/AppNotes.aspx?appnote=en020511) paper about PwLI which represents the term `( x - x0 )`. And the slope times the span is called the "offset".
```
    /--------------------offset--------------\
    /-----------slope-----------\   /--span--\
y = ( ( y1 - y0 ) / ( x1 - x0 ) ) * ( x - x0 ) + y0;
```
The linearly-interpolated functions ran only about half as fast as their midpoint-interpolated counterparts, but were roughly 250 times as accurate.

### Changing from a uniform to a non-uniform distribution of x-values

It may have occurred to you at this point, after reflecting on the sin function for so long, that the slope of sin near it's zero-crossing points is very nearly linear, but near it's maxima and minima the slope changes very quickly, and it might be possible to spread out the points near the zero-crossing and bunch them up near the maxima and minima, thereby achieving the same or better accuracy with fewer overall points. It is this "spreading out" and "bunching up" of x-values that I call a "non-uniform distribution" and it can, in fact, offer the same or better accuracy with fewer points (though, as usual, at the cost of added complexity).
```
// This is how our LUT is indexed right now: evenly spaced points across the range [0, 2*PI*64].
                                       (PI/2)*64 ~= 100.5
                                               |
                                              \|/
    0    1    2    3    4    ....    99    100    101    102    ....    403

// But since sin changes slowly around 0 and much more rapidly around maxima/minima (like PI/2), we'd like to do something like this:
// (Notice how the index changes quickly around 0, where sin is very linear, and slowly around PI/2, where sin is changing quickly)
                                            (PI/2)*64 ~= 100.5
                                                    |
                                                   \|/
    0    3    5    6    7    ....    99.5  100    100.5  101    ....    403
```
Of course, an array is incapable of having fractional indices like these. So to implement this new-and-improved LUT, we need to move to a data structure called a "dictionary" or "key-value" list. In those lists, the indices aren't implicit (as they are in an array), they are explicit: a stated value coupled with their "value" or "definition" (or "y-value", in our case). In C, this is usually accomplished by creating a struct (which contains our desired keys and values) and then an array of those structs.
```
// Before, using an array: "Keys" are implicit and correspond to the indices of the array. The "values" are stored in the array.
//
                                     //          sin(0/64)            sin(1/64)            sin(2/64)
static double sinTable_double[...] = { 0.00000000000000000, 0.01562436422488340, 0.03124491398532610 ... };

// After, using a dictionary (implemented as an array of structs): "Keys" are specified by the "x" value in the struct. "Values" are
// specified by the "y" value in the struct. The array indices aren't used.
//
typedef struct point_double_t
{
    double x;
    double y;
} point_double_t;

static point_double_t nonUniform_double_0dot007error[...] = 
{
    //------"keys"--------   ------"values"-------
    //-----x-values-------   --y-values (sin(x))--
    { 0.000000000000000000,  0.000000000000000000 },
    { 0.246907827863357000,  0.244406737256656000 },
    ...
    { 6.036277748382230000, -0.244406476350662000 },
    { 6.283185307179590000,  0.000000000000000000 }
};
```
We can't simply convert our input to an integer now to get the correct array element, though; we'll need to search for it in the LUT. Since the LUT is ordered by x-value from least to greatest, however, we get the small benefit of being able to perform a binary seach (instead of a linear search). To do this, the code uses placeholders for the smallest, greatest, and middle array indices called `low`, `high`, and `mid`. First the code checks if the x-value at `mid` is less than the input AND if the x-value at `mid + 1` is GREATER than the input; if it is, then we've found our point and we can `break` out of the `while` loop. If not, the code checks if the value at `mid` is GREATER than our input and, if it is, it moves `high` to `mid`, effectively cutting off the top half of our LUT. If the value at `mid` is LESS than our input, then the code moves `low` to `mid`, effectively cutting off the top half of our LUT. In this manner, the code "cuts" the LUT in half after every search until it finds the correct value. Ex:
```
1) Assume we have a LUT with 12 elements; the values low, mid, and high start out as below.
       low                                       mid                                       high
        |                                         |                                         |
       \|/                                       \|/                                       \|/
Index:  0      1      2      3      4      5      6      7      8      9     10     11     12
Value:  0    0.1    0.1    0.2    0.3    0.5    0.8    1.3    2.1    3.4    5.5    8.9   14.4

2) Assume our input is in between the values at locations 2 & 3.
       low             input(0.11)               mid                                       high
        |               |                         |                                         |
       \|/             \|/                       \|/                                       \|/
Index:  0      1      2      3      4      5      6      7      8      9     10     11     12
Value:  0    0.1    0.1    0.2    0.3    0.5    0.8    1.3    2.1    3.4    5.5    8.9   14.4

3) The first iteration of the while loop finds that the value of input is less than the value of the array at mid. So it "cuts" the table in half by moving "high" to mid.
       low     input(0.11)   mid                high
        |               |    |                    |
       \|/             \|/  \|/                  \|/
Index:  0      1      2      3      4      5      6      7      8      9     10     11     12
Value:  0    0.1    0.1    0.2    0.3    0.5    0.8    1.3    2.1    3.4    5.5    8.9   14.4

4) The second iteration of the while loop finds again that the value of input is less than the value of the array at mid. So it "cuts" the table in half again.
       low           mid         input(0.11)     high
        |             |                |          |
       \|/           \|/              \|/        \|/
Index:  0             1             2             3             4             5             6
Value:  0           0.1           0.1           0.2           0.3           0.5           0.8

5) The third iteration of the while loop finds that, now, the value of the input is greater than the value of the array at mid. So it "cuts" the table in half again, this time moving "low" to mid.
                     low          mid input(0.11) high
                      |             |  |          |
                     \|/           \|/\|/        \|/
Index:  0             1             2             3             4             5             6
Value:  0           0.1           0.1           0.2           0.3           0.5           0.8

6) Finally, since array[mid] < input AND array[mid+1] > input, we've found our desired indices.
```
At this point, we would compute the linear interpolation as above.

To determine what the points in our LUT need to be, we need to return to our error equation: <img src="https://github.com/nathancharlesjones/Look-up-Table-Examples/blob/master/02_Other-Sin-Improvements/docs/Equation_Error_Linear-Interpolation.png" width="150"> (we'll assume that we're performing a linear interpolation). Recall that `h` is the difference between our two adjacent x-values, in other words, `x1 - x0`. This time around, "error" is a known quantity (we're trying to determine where to place our points in order to meet a certain maximum error), so what we'll need to do is solve this equation in terms of `e` and either `x0` or `x1` (we assume that at least one of them is known) in order to find the other (either `x1` or `x0`).

Let's assume, to start, that we're either in the range `[PI/2, PI]` or `[3*PI/2, 0]` (we'll see why this is important in a minute). Blessedly, the absolute value of the double-derivative of sin(x) is just sin(x) (`|sin''(x)| == sin(x)`), so we'll only need to find the maximum value for sin over each segment in order to evaluate the second term. Under the conditions above, the maximum absoulte value for sin over our segment will always be equal to `x0` (since `|sin(x0)| >  |sin(x1)|` for `x0 < x1` in the range `[PI/2, PI]` or `[3*PI/2, 0]`). If we substitute `x1 - x0` for `h` above and solve for `x1`, we get the following equation:
```
                /-----------
x1 = x0 +      /  error * 8
          --  /  ----------
            \/     sin(x0)
```
If we assume that we start with a point at both `PI/2` and `3*PI/2`, then all we need to do to get our non-uniform distribution of points (in the ranges above) is to calculate `x1`, set `x0` equal to `x1` and repeat (that is, calculate the n+1 point given the nth point).

For example, assume we want to determine our first point (after `PI/2`) with a maximum error of 0.00003 (not coincidentally the maximum error using our linear interpolations above). We would solve for it like this:
```
                /-----------                    /-----------
x1 = x0 +      /  error * 8   =  (PI/2) +      / 0.00003 * 8  =  1.58628826
          --  /  ----------               --  /  -----------
            \/     sin(x0)                  \/    sin(PI/2)
```
Then we would set `x0` equal to 1.58628826 and solve again for the next point.

Things get a litle tricky, however, if we're in the range `[0, PI/2]` or `[PI, 3*PI/2]`. The reason is this: the maximum value for the absolute value of sin over any of our segments is now not `sin(x0)` but `sin(x1)`. Substituting as above for our error equation, we get the following result:
```
error * 8 = ( ( x1 - x0 )^2 ) * sin( x1 )
```
Unfortunately, I couldn't find a way to solve that equation for `x1`. The trick, then, is not to iterate FORWARDS from `0` to `PI/2`, but BACKWARD from `PI/2` to `0`. In so doing, we try to solve for `x0` given `x1`, and that is an equation we can solve, since now `|sin(x1)| >  |sin(x0)|` for `x0 < x1` in the range `[0, PI/2]` or `[PI, 3*PI/2]`:
```
                /-----------
x0 = x1 -      /  error * 8
          --  /  ----------
            \/     sin(x1)
```
If we assume, again, that we start with a point at both `PI/2` and `3*PI/2`, then all we need to do to get our non-uniform distribution of points (in the ranges above) is to calculate `x0`, set `x1` equal to `x0` and repeat (that is, calculate the n-1 point given the nth point).

For example, assume we want to determine the point BEFORE `PI/2`, with a maximum error of 0.00003. We would solve for it like this:
```
                /-----------                    /-----------
x0 = x1 -      /  error * 8   =  (PI/2) -      / 0.00003 * 8  =  1.555304393
          --  /  ----------               --  /  -----------
            \/     sin(x1)                  \/    sin(PI/2)
```
Then we would set `x1` equal to 1.555304393 and solve again for the point before THAT.

There is a small program in the `tools` folder called `non-uniform-distribution-helper.c` that performs these steps for you and prints out the LUT for any desired error, formatted for easy copy/paste into an array definition. There is no Makefile for this; simply change `max_error` to your desired maximum error, compile using GCC with the command `gcc -I./ -Og -ggdb non-uniform-distribution-helper.c -lm`, and run the resulting program (called `a.out`).

Acheiving the same maximum error as our LUTs with linear interpolation above (0.00003) only requires 317 elements instead of 404, a memory savings of around 21.5%. To save myself the time of implementing those LUTs, however, I built tables with a maximum error of 0.007, on par with our "standard" LUTs using midpoint interpolation (not a fair comparison, I know, since we used midpoint interpolation before and linear interpolation with the non-uniform LUTs). Those tables only required 25 elements, a memory savings of almost 94%! They ran quite a bit more slowly than the LUTs using linear interpolation (I'm guessing mostly as a result of needing to perform a binary search to match each input before the linear interpolation could be performed).

For any other function, `f(x)`, here are the steps to repeat what we've done above:
1. Determine the second derivative of `f(x)` (`f''(x)`).
2. Identify where the absolute value of the second derivative is DECREASING; compute x1 from x0 (i.e. iterate FORWARD) using the following equation:
```
                /-----------
x1 = x0 +      /  error * 8
          --  /  ----------
            \/     sin(x0)
```
3. Identify where the absolute value of the second derivative is INCREASING; compute x0 from x1 (i.e. iterate BACKWARD) using the following equation:
```
                /-----------
x0 = x1 -      /  error * 8
          --  /  ----------
            \/     sin(x1)
```

### Comparing to the polynomial approximations

Another way to implement the sin (and other trig) function is by approximating it with a high-order polynomial. This is conceptually similar to the idea of a linear interpolation except that higher-order polynomials can be accurate over a much wider range than a simple line can be. Jack Ganssle discusses this approach in-depth [here](http://www.ganssle.com/approx.htm) and I'll not reiterate it. They were included to mostly for my own curiousity about their relative performance.

From the table above, we can see that the primary benefit to using a polynomial approximation as compared to the library sin is a reduction in memory size, since the functions all ran about as quickly as the library sin function. They also offer much greater accuracy than could be reasonably achieved with one of our LUTs. If the precision of the library sin function is limited only by the precision of the double data type (roughly 1.1e-16 in the range [-1, 1]), then the final polynomial approximation, `Sin_121`, is the only one to come close to achieving a similar level of accuracy (and a LUT with that level of accuracy would be many hundreds of thousands or millions of elements long!).