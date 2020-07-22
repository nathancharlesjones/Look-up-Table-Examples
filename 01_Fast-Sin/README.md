# Fast Sin

## What is it?
"Fast Sin" is a demonstration of using a look-up table (LUT) to improve the execution time of the library sin function. It also demonstrates several aspects of professional quality code, such as using asserts, enforcing various compiler optimizations, building for multiple targets, and having a well-structured Makefile.

The following table summarizes the results.
|Function|Memory usage (bytes)|Absolute Error|Percent Error|Execution time|
|---|---|---|---|---|
|sin|5368|N/A|N/A|~63-64 us|
|sin_LUT|2936|0.0028|1.8-2.0|~20.4 us|

## How do I use it?

## How does it work?
