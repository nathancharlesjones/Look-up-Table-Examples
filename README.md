# Look-up Tables (LUTs)

## Description
A “look-up table” (LUT) is a programming idiom in which the pre-computed values of long or complex functions are stored in an array or other memory structure, thereby allowing quick and/or straightforward access to those values using a “key” (via array indexing or searching). In this manner, a LUT typically saves a program computation time at the cost of the memory used to store the table. LUTs can be used to replace or implement computationally intensive math functions, complex conditionals, the encoding and decoding of data, fast generation of arbitrary waveforms or bitstreams, jump tables, table-driven finite state machines, simple polymorphic structures, and digital logic blocks (that would otherwise require dedicated external hardware), among other things.

### Example: Fast Sin
The most common usage of a look-up table is to implement simpler and "just accurate enough" versions of math functions like sin or exp, as shown below. In the table below, each element at position “X” is assigned the value of “sin(X)”, where “X” is a value in degrees.
```
const double sin_LUT[360] = {
    //              sin(0)               sin(1)
                        0,  0.0174524064372835,
    //              sin(2)               sin(3)
       0.0348994967025010,  0.0523359562429438,
    // ... Many more rows
    //            sin(356)             sin(357)
      -0.0697564737441248, -0.0523359562429444,
    //            sin(358)             sin(359)
      -0.0348994967025008, -0.0174524064372845 };
```
Having defined the array “sin_LUT” like this, our program can find the sin of any integer angle from 0 to 359 degrees by simply executing a memory access, instead of having to compute the sin directly (which may take many clock cycles).
```
double sin_4 = sin_LUT[4]     // Equivalent to sin(4*PI/180)
double sin_354 = sin_LUT[354] // Equivalent to sin(354*PI/180)
```
A comparison of a slightly modified version of this implementation to that of the sin function from the “nanolib” C library can be found in the first code example, [01_Fast-Sin](https://github.com/nathancharlesjones/Look-up-Table-Examples/tree/master/01_Fast-Sin). That code was compiled for an STM32F1 running at 72 MHz using GCC 6.3.1 on Ubuntu; there’s also code to run that same example on an x86 so you don’t need an STM32F1 in order to test it, though the results are less drastic. It shows that although the sin LUT has a maximum absolute error of around 0.0077 (owing to the rounding of input values to the nearest integer in order to find the nearest array index), it runs over 7 times faster than the library sin function. Interestingly, the sin LUT also takes much less code space (though this is not always the case), likely owing to the fact that the library sin function requires software floating point libraries which the LUT does not. (With a few modifications, we can further improve execution time (to 0.733 us, 64 times faster than sin), absolute error (to 0.00003, 254 times more accurate than this example), and also memory usage (to 824 bytes, nearly 1/7 the size of sin), though not all at the same time; see [02_Other-Sin-Improvements](https://github.com/nathancharlesjones/Look-up-Table-Examples/tree/master/02_Other-Sin-Improvements).)
|Function|Memory usage (bytes)|Max Absolute Error|Percent Error|Execution time|
|---|---|---|---|---|
|sin|5368|N/A|N/A|\~45.8 us|
|sin_LUT|3272|\~0.0025|\~1.7|\~3.79 us|

Lest the idea of a 1.7% error disagree with you, consider this (semi)realistic scenario: a 1-foot long lever is connected to the shaft of a servo motor. The lever only travels from 0 to 90 degrees; that is, from horizontal to vertical and you wish to determine the vertical displacement of the highest point of the lever based on a given angle. You can compute this by multiplying the length of the lever (1 foot) by the sin of the angle formed by it and a horizontal plane, which you can measure from the servo motor. In this scenario, a 1.7% error still gives you an answer that is accurate to, at worst, about one-fifth of an inch or 5 mm. The average absolute error of 0.0025 means that most of your calculations will be off by less than 0.76 mm. For all but a few applications, this resolution is probably entirely sufficient. Of course, there are other ways to improve the accuracy of a LUT if it is needed (see below).

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

## Defining a Look-up Table

### Storage Structure

Based on our broad definition that a LUT simply stores a value for future use, there are lots of ways to structure them. The simplest methods are ones you've likely already seen, though they aren't even really tables at all. They are macros (i.e. `#define PI 3.14159f`), constant variables (i.e. `const double pi = 3.14159;`), and enumerations (i.e. ). In each case, 
Const/#define, enum, Array, Linked

### Scope

It is recommended that LUTs be defined from inside an access function (see below). Although a LUT can be defined anywhere in a program, placing it in a function allows the developer (that's you) the ability to change many aspects of how the LUT is defined and accessed in the future without needing to change a lot of other code. The function effectively encapsulates the details of how your LUT is implemented. Declaring the LUT to have file scope is a suitable alternative.
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

// This is a suitable alternative.
// sin_LUT.c:
// 
static const double sin_table[360] = {...};
double sin_LUT( double degrees ){...};
```
Also, always ensure index values fall within a valid range before accessing the LUT. This is called "bounds checking" and it ensures you don't accidentally read from memory that lies beyond the table. The only exception to this is if the size of your LUT exactly matches the max size for a standard unsigned integer data type (i.e. it has exactly 2<sup>8</sup>, 2<sup>16</sup>, or 2<sup>32</sup> elements) AND an unsigned integer of the same width is used to access the table (i.e. a uint8_t is used to access a table with 256 elements). In that case, it can be said that the compiler performs the bounds checking for you.
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
const int something
```
Use the "static" qualifier, absent any "const", to inform the linker it should place the LUT in RAM. Take care not to use BOTH "static" and "const" together, since "const" will take precedence and the array will be placed in ROM. Ex:
```
// Placed in RAM
static int something
```
Without either qualifier, the LUT will be placed in RAM and space will be allocated on the stack for it

### Data type

LUTs can hold any type of data, including integers, floats, doubles, fixed-point numbers, chars, strings, pointers, or user- or library-defined data types, though the most straightforward implementations will hold only one of these types. Using unions, a LUT may even hold a variety of these data types.
```
// Code here
```

### Addressing Method

There are four ways to address the elements in a LUT: directly, indirectly, by hashing, or by searching. All four methods can be used to address arrays, though the first three are the most common. Dynamically-allocated memory structures, like a linked list, can only be addressed via the fourth method.

When a LUT is directly addressed, it’s elements are retrieved by a number that denotes their exact position in memory or in the LUT. This is typically accomplished using array indices.
```
// Code here
```
When a LUT is indirectly addressed, it’s elements are retrieved by first looking up their position or index in another LUT. This can be helpful when a LUT is needed to store a large data type for non-contiguous values; instead of allocating space for every value at every position (wasting much space), a second LUT (holding just the index of the desired value) can be allocated to hold the indices while the original LUT holds just the data desired.
```
// Code here
```
A “hash” is a function that transforms an input value or “key” into an array index. For integers, this can be as simple as “index = key % length_of_LUT”. For strings, this could be XOR-ing each character in the string. Or it could even be application-specific: sin range reduction. A hash is most useful when the keys, the items to be looked up, don’t exactly map to the array’s indices (i.e. aren’t all positive integers that start at 0). For any hash function and 
```
// Code here
```
In the event the LUT is implemented as a dynamically-allocated memory structure (in which we cannot directly address any of its elements) or if the keys to the LUT are such that they cannot be easily hashed, then we must perform a search, comparing our “key” to the keys in the LUT until we find a match, and only then retrieving the associated value. A sequential search is the most straightforward and also one of the slowest search methods. A binary search is much faster, particularly for large LUTs, but it requires that the LUT be sorted. There are many more and faster search algorithms available to us if the LUT is sorted. For more about searching and sorting, see here. Searching will inevitably take longer than a direct or indirect addressing method, however.
```
// Code here
```

### Dimensions

A LUT can have one dimension or many, depending on how many keys are provided for the look-up. For arrays, this corresponds directly to the dimensions of the array. For dynamically-allocated memory structures, this would correspond to the number of fields being compared when the search is conducted. Multi-dimensional LUTs are often put to use implementing complex conditionals or jump tables (see below).
```
// Code here
```

## Advanced

Asf

### Table filled-at run-time

Asdf

### Dynamically-allocated LUTs

Asdf

### Arbitrary-type LUTs

asdf

## Uses

### Fast Math

Asdf

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

