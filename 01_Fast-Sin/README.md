# Fast Sin
## Contents
1. What is it?
2. How do I use it?
3. How does it work?

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
```
$ make BUILD=debug TARGET=x86
```
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
```
$ make BUILD=debug TARGET=x86 clean
```
### On an STM32
1. Procure an STM32F103C8T6, sometimes called a "Blue Pill".
2. After downloading the Git repo, navigate to the folder "01_Fast-Sin".
2. Decide whether you want a "debug" or "release" build (the only difference which optimization flag is passed to the compiler) and type "make BUILD={debug or release} TARGET=STM32F1" into a shell (replacing "{debug or release}" with either "debug" or "release", of course).
```
$ make BUILD=debug TARGET=STM32F1
```
3. The executable is called "{debug or release}_STM32F1.elf" and it will be placed in a folder called "build/{debug or release}_STM32F1"
4. Connect your STM32F1 to your computer using your debugger of choice (e.g. ST-Link, J-Link, etc) and start a GDB server. If you have previously downloaded STM32CubeIDE, you can start a GDB server from the command line using the script "Start-ST-Link-GDB-Server_v2" (after updating the file paths appropriately).
```
$ ./Start-ST-Link-GDB-Server_v2.sh
```
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

For more information about how the Makefile works, see [here](https://github.com/nathancharlesjones/Generic-Makefile-based-Project-for-x86-and-STM32F1).

## How does it work?
Let's start with "sin_LUT.c", the heart (if not the meat) of this little example. In lines X-X, we define an array of doubles called "sin_table".
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
At each index X is stored the value of sin(X/64), allowing us to find the value of any radian angle from 0 to 2*PI simply by accessing the array (e.g. "sin[50]" is equivalent to "sin(50/64)"; notice how the first statement is merely an array access while the second is a library function call). The array size (which may seem odd) was chosen to be a power-of-two multiple of 2*PI (or as close to it as makes sense) whose value was in the low-hundreds. The power-of-two multiple criteria was imposed to make converting from radians to array indices easier (power-of-two multplies are far easier to execute in software than any of type of multiplication). The "value in the low-hundreds" criteria was imposed in order to have enough data points to keep the average overall error low. A table with only 10 elements will have much error, since the data points are so far apart; however, a table with 1000 or more elements would take up too much space in memory. The sin_table array is made static and a function is written to access it so that no other code can depend on our implementation of sin_table; it can change in the future and as long as our function still takes a double as input and returns a double that is the sin of the input, then no other code needs to change.
```
double sin_LUT(double radians)
{
	...
    // Not shown: Converting "radians" to an array index and also ensuring that it's a valid index
    ...
	return sin_table[ idx ];
}
```
This function converts the input to an appropriate integer and also make sure that it falls within the array bounds (see the code comments for additional details).

Now let's move to "main.c". The important stuff happens in lines 50-60 and 64-74. In those blocks, we're repeatedly creating a random input value, storing the current "system time" into a variable called p_start, calling the function we want to profile, storing the updated system time into a variable called p_end, and computing the difference between the two times (in nanoseconds). That process happens "testIterations" numbers of times and then the sum is divided by "testIterations" to determine the average execution time for the function. The time-related functions are defined elsewhere in the project, meaning that they are "hardware independent", so far as the main function is concerned. They are defined as returning an error value: 0 for success and non-zero for an error. So these blocks also ASSERT that the return value isn't an error code; the ASSERT macro is defined elsewhere (don't worry, we'll get to that too).

Lines 78-105 compute the error associated with the sin LUT.

At the top and bottom of this file are a few pragmas, or compiler directives, that instruct GCC to optimize this file at "-O0", the lowest level of optimization, regardless of how the rest of the project is being optimized. This is required so that the compiler doesn't accidentally optimize away later calls
