# Look-up Tables (LUTs)

## Description
A “look-up table” (LUT) is a programming idiom in which the pre-computed values of long or complex functions are stored in an array or other memory structure, thereby allowing quick and/or straightforward access to those values using a “key” (via array indexing or searching). In this manner, a LUT typically saves a program computation time at the cost of the memory used to store the table. LUTs can be used to replace or implement computationally intensive math functions, complex conditionals, the encoding and decoding of data, fast generation of arbitrary waveforms or bitstreams, jump tables, table-driven finite state machines, simple polymorphic structures, and digital logic blocks (that would otherwise require dedicated external hardware), among other things.

### Example: Fast Sin
The most common usage of a look-up table is to implement simpler and "just accurate enough" versions of math functions like sin or exp, as shown below. In the table below, each element at position “X” is assigned the value of “sin(X)”, where “X” is a value in degrees.
```
const double sin_LUT[360] = {
    //       sin(0*PI/180)        sin(1*PI/180)
                        0,  0.0174524064372835,
    //       sin(2*PI/180)        sin(3*PI/180)
       0.0348994967025010,  0.0523359562429438,
    // ... Many more rows
    //     sin(356*PI/180)      sin(357*PI/180)
      -0.0697564737441248, -0.0523359562429444,
    //     sin(358*PI/180)      sin(359*PI/180)
      -0.0348994967025008, -0.0174524064372845 };
```
Having defined the array “sin_LUT” like this, our program can find the sin of any integer angle from 0 to 359 degrees by simply executing a memory access, instead of having to compute the sin directly (which may take many clock cycles).
```
double sin_4 = sin_LUT[4]     // Equivalent to sin(4*PI/180)
double sin_354 = sin_LUT[354] // Equivalent to sin(354*PI/180)
```
A comparison of a slightly modified version of this implementation to that of the sin function from the “nanolib” C library can be found in the first code example, [01_Fast-Sin](https://github.com/nathancharlesjones/Look-up-Table-Examples/tree/master/01_Fast-Sin). That code was compiled for an STM32F1 running at 72 MHz using GCC 6.3.1 on Ubuntu; there’s also code to run that same example on an x86 so you don’t need an STM32F1 in order to test it, though the results are less drastic. It shows that although the sin LUT has a maximum absolute error of around 0.0077 (owing to the rounding of input values to the nearest integer in order to find the nearest array index), it runs over 12 times faster than the library sin function. Interestingly, the sin LUT also takes much less code space (though this is not always the case). (With a few modifications, we can further improve execution time (to 0.733 us, 64 times faster than sin), absolute error (to 0.00003, 254 times more accurate than this example), and also memory usage (to 824 bytes, nearly 1/7 the size of sin), though not all at the same time; see [02_Other-Sin-Improvements](https://github.com/nathancharlesjones/Look-up-Table-Examples/tree/master/02_Other-Sin-Improvements).)
|Function|Memory usage (bytes)|Max Absolute Error|Percent Error|Execution time|Speed-up factor|
|---|---|---|---|---|---|
|Library sin|5480|N/A|N/A|\~47 us|N/A|
|LUT sin|3484|\~0.0076|\~2.5|\~6.7 us|7.04x|

Lest the idea of a maximum error of 0.0077 disagree with you, consider this (semi)realistic scenario: a 1-foot long lever is connected to the shaft of a servo motor. The lever only travels from 0 to 90 degrees; that is, from horizontal to vertical and you wish to determine the vertical displacement of the highest point of the lever based on a given angle. You can compute this by multiplying the length of the lever (1 foot) by the sin of the angle formed by it and a horizontal plane, which you can measure from the servo motor. In this scenario, an absolute error of 0.0077 means that most of your calculations will be off by less than one-tenth of an inch or 2.3 mm. For all but a few applications, this resolution is probably entirely sufficient. Of course, there are other ways to improve the accuracy of a LUT if it is needed (see below).

## Table of Contents

1. [Description](https://github.com/nathancharlesjones/Look-up-Table-Examples#description)
	1. [Example](https://github.com/nathancharlesjones/Look-up-Table-Examples#example-fast-sin)
2. [Defining a Look-up Table](https://github.com/nathancharlesjones/Look-up-Table-Examples#defining-a-look-up-table)
	1. [Storage Structure](https://github.com/nathancharlesjones/Look-up-Table-Examples#storage-structure)
	2. [Scope](https://github.com/nathancharlesjones/Look-up-Table-Examples#scope)
	3. [Location in Memory](https://github.com/nathancharlesjones/Look-up-Table-Examples#location-in-memory)
	4. [Data type](https://github.com/nathancharlesjones/Look-up-Table-Examples#data-type)
	5. [Addressing Method](https://github.com/nathancharlesjones/Look-up-Table-Examples#addressing-method)
	6. [Dimensions](https://github.com/nathancharlesjones/Look-up-Table-Examples#dimensions)
3. [Advanced](https://github.com/nathancharlesjones/Look-up-Table-Examples#advanced)
	1. [Table filled-at run-time](https://github.com/nathancharlesjones/Look-up-Table-Examples#table-filled-at-run-time)
	2. [Dynamically-allocated LUTs](https://github.com/nathancharlesjones/Look-up-Table-Examples#dynamically-allocated-luts)
	3. [Arbitrary-type LUTs](https://github.com/nathancharlesjones/Look-up-Table-Examples#arbitrary-type-luts)
4. [Uses](https://github.com/nathancharlesjones/Look-up-Table-Examples#uses)
	1. [Fast Math](https://github.com/nathancharlesjones/Look-up-Table-Examples#fast-math)
	2. [Bit Twiddling and Format Conversion](https://github.com/nathancharlesjones/Look-up-Table-Examples#bit-twiddling-and-format-conversion)
	3. [Digital Logic](https://github.com/nathancharlesjones/Look-up-Table-Examples#digital-logic)
	4. [Mapping Metadata](https://github.com/nathancharlesjones/Look-up-Table-Examples#mapping-metadata)
	5. [Complex Conditionals](https://github.com/nathancharlesjones/Look-up-Table-Examples#complex-conditionals)
	5. [Jump Tables and Table-driven Finite State Machines](https://github.com/nathancharlesjones/Look-up-Table-Examples#jump-tables-and-table-driven-finite-state-machines)
	6. [Fast Output](https://github.com/nathancharlesjones/Look-up-Table-Examples#fast-output)
	7. [Dictionaries](https://github.com/nathancharlesjones/Look-up-Table-Examples#dictionaries)
5. [References](https://github.com/nathancharlesjones/Look-up-Table-Examples#references)

## Defining a Look-up Table

### Storage Structure

Based on our broad definition that a LUT simply stores a value for future use, there are lots of ways to structure them. The simplest methods are ones you've likely already seen, though they aren't even really tables at all. They are macros (i.e. `#define PI 3.14159f`), constant variables (i.e. `const double pi = 3.14159;`), and enumerations (i.e. `enum { NO_ERROR, BAD_INPUT, MATH_OVERFLOW } errorCode_t`). In each case, the variable handle (`PI`, `pi`, `errorCode_t`) points to the value that is commonly needed in a program and provides a quick and convenient reference to it.

A more common structure for a LUT is an array, in which the array indices are used as "keys" to get at the array values. These work particularly well when the keys constitute a set of positive integers that starts at 0.
```
// LUT of sin values with inputs as integer degrees
//
const double sin_LUT[360] = { // sin(0*PI/180), sin(1*PI/180), ... , sin(359*PI/180) };

// Input (in degrees) maps directly on to array indices
//
double sin_4_deg = sin_LUT[4]     // Equivalent to sin(4*PI/180)
```
For any other arrangement of keys, the input must be converted or "hashed" to the correct array index. (In fact, the mapping of the input directly on to the array indices is sometimes called the "trivial hash".)
```
// LUT of sin values with inputs as floating-point radians; LUT elements occur at intervals of 1/64 radians in order to improve the accuracy of the table
//
static const double sin_table[SIN_LUT_SIZE] = { // sin(0/64), sin(1/64), sin(2/64), ... , sin(403/64) };

// Input (in radians) is multipled by 64 and then truncated in order to get the array index; the multiplication and truncation are the "hash"
//
int idx = (int)( radians * 64 );
return sin_table[ idx * 64 ]  // Equivalent to sin(radians)
```
When the keys don't map easily to array indices, a "dictionary" or "key-value list" can be used. In these lists, the indices aren't implicit (as they are in an array), they are explicit: a stated value coupled with their "value" or "definition". In C, this is usually accomplished by creating a struct (which contains our desired keys and values) and then an array of those structs.
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

### Scope

It is recommended that LUTs be defined from inside an access function. Although a LUT can be defined anywhere in a program, placing it in a function allows the developer (that's you) the ability to change many aspects of how the LUT is defined and accessed in the future without needing to change a lot of other code. The function effectively encapsulates the details of how your LUT is implemented. Declaring the LUT to have file scope is a suitable alternative.
```
// Do THIS...
double sin_LUT( double degrees )
{
    // sin_table is restricted to function-level scope
    // Provided the function signature stays the same (double in, 
    // double out), we can change anything about this LUT in the 
    // future (such as how many entries are in the array, changing the 
    // date type of the array, etc) without needing to change any 
    // other code.
    //
    const double sin_table[360] = {...};
    return sin_table[(int)degrees];
}

// ...NOT this
// sin_table is now a global variable and every other part of the 
// program knows exactly how it's implemented. We couldn't change 
// anything about it without needing to change every other part of our 
// program that depended on sin_table being a 360-element long array 
// of doubles.
//
const double sin_table[360] = {...};

// This is a suitable alternative. "sin_table" is declared "static",
// meaning that only functions inside "sin_LUT.c" have access to it.
// sin_LUT.c:
// 
static const double sin_table[360] = {...};
double sin_LUT( double degrees ){...};
```
Also, always ensure index values fall within a valid range before accessing the LUT. This is called "bounds checking" and it ensures you don't accidentally read from memory that lies beyond the table. The only exception to this is if the size of your LUT exactly matches the max size for a standard unsigned integer data type (i.e. it has exactly 2<sup>8</sup>, 2<sup>16</sup>, or 2<sup>32</sup> elements) AND an unsigned integer of the same width is used to access the table (i.e. a uint8_t is used to access a table with 256 elements). In that case, every possible input value is a valid index and bounds-checking is superfluous.
```
// The above function should have been written like this:
//
double sin_LUT( double degrees )
{
    const double sin_table[360] = {...};

    // Trigger an error if the input value doesn't map to a valid 
    // array index.
    //
    ASSERT( 0 <= x );
    ASSERT( x <= 359 );

    return sin_table[(int)degrees];
}
```

### Location in Memory

Whether the look-up table is placed in RAM or ROM depends on (1) how much space is available in each memory segment and (2) whether the application can afford the latency (if any) inherent to ROM accesses. LUTs should not be allocated on the stack (that is, re-created every time the function is called) except in very rare circumstances.

Use the "const" qualifier to inform the linker it should place the LUT in ROM. This is the most common scenario. Ex:
```
// Placed in ROM
const int myLUT[NUMBER_OF_ELEMENTS] = {...};
```
Leave off "const" entirely in order to inform the linker to place the LUT in RAM. It's recommended, however, to add the "static" qualifier, in this case, for two reasons: (1) if the LUT is defined in a file, but not inside any function, the "static" qualifier helps encapsulate the implementation of the LUT (by only allowing functions inside the file to reference it, but not any others) and (2) if the LUT is defined in a function, the "static" qualifier informs the compiler that the LUT retains its values between function calls, so the compiler won't allocate room on the stack for the LUT every time the function gets called.
```
// Placed in RAM (but not recommended)
int myLUT[NUMBER_OF_ELEMENTS] = {...};

// Recommended
static int myLUT[NUMBER_OF_ELEMENTS] = {...};
```

### Data type

LUTs can hold any type of data, including integers, floats, doubles, fixed-point numbers, chars, strings, pointers, or user- or library-defined data types, though the most straightforward implementations will hold only one of these types. Using unions, a LUT may even hold a variety of these data types.

### Addressing Method

There are four ways to address the elements in a LUT: directly, indirectly, by hashing, or by searching. All four methods can be used to address arrays, though the first three are the most common.

When a LUT is directly addressed, it’s elements are retrieved by a number that denotes their exact position in memory or in the LUT. This is typically accomplished using array indices.
```
// Code here
```
When a LUT is indirectly addressed, it’s elements are retrieved by first looking up their position or index in another LUT. This can be helpful when a LUT is needed to store a large data type for non-contiguous values; instead of allocating space for every value at every position (wasting much space), a second LUT (holding just the index of the desired value) can be allocated to hold the indices while the original LUT holds just the data desired.
```
// Code here
```
A “hash” is a function that transforms an input value or “key” into an array index. For integers, this can be as simple as `index = (int)( input * 64 )` or `index = key % length_of_LUT`. For strings, this could be XOR-ing each character in the string. A hash is most useful when the keys, the items to be looked up, don’t exactly map to the array’s indices (i.e. aren’t all positive integers that start at 0).
```
// Code here
```
In the event the keys to the LUT are such that they cannot be easily hashed, then we must perform a search, comparing our “key” to the keys in the LUT until we find a match, and only then retrieving the associated value. A sequential search is the most straightforward and also one of the slowest search methods. A binary search is much faster, particularly for large LUTs, but it requires that the LUT be sorted. There are many more and faster search algorithms available to us if the LUT is sorted. For more about searching and sorting, see here. Searching will inevitably take longer than a direct or indirect addressing method, however.
```
// Code here
```

### Dimensions

A LUT can have one dimension or many, depending on how many keys are provided for the look-up. Multi-dimensional LUTs are often put to use implementing complex conditionals or jump tables.
```
// Code here
```

## Advanced

The following techniques are slightly more advanced (and therefore less commonly used) versions of the LUTs described above.

### Table filled-at run-time

For tables that are located in RAM, it is possible to write a function to fill them with the correct values at the start of the program, thereby saving the developer (that's you again) the agony of having to hand-type (or even copy, paste, and format) an entire LUT. The upside is that this implementation is far more straightforward and doesn't suffer from the risk of a typo causing errant bugs (as a hand-typed array of many hundreds of elements might). The downside is that the table must now be stored in RAM, which is often a more precious resource in embedded systems than is ROM. Additionally, the LUT must be of a form that the target processor can actually compute at run-time, which may not be the case for extremely complex LUTs or for LUTs whose data is truly arbitrary (e.g. based on a table of sensor compensation values).
```
static double sinTable_double[SIN_LUT_SIZE];

void init_sinLUT(void)
{
    for( int idx = 0; idx < SIN_LUT_SIZE; idx++)
    {
        double radians = ( (double) idx ) / 64.0;
        sinTable_double[idx] = sin( radians );
    }
}
```

### Dynamically-allocated LUTs

Asdf

### Arbitrary-type LUTs

asdf

## Uses

### Fast Math

The most common usage for LUTs is to implement faster and "just-accurate-enough" versions of complex math functions. [02_Other-Sin-Improvements](https://github.com/nathancharlesjones/Look-up-Table-Examples/tree/master/02_Other-Sin-Improvements) shows a number of different LUTs that implement the sin function that are, alternately, 64 times faster and 1/7 the code size than the library sin implementation.

### Bit Twiddling and Format Conversion

Asdf

### Digital Logic

adf

### Mapping Metadata

Asdf

### Complex Conditionals

asdf

### Jump Tables and Table-driven Finite State Machines

Asdf

### Fast Output

Asdf

### Dictionaries

asdf

## References

- 