#include <math.h>
#include <stdio.h>
#include "sin_lut.h"

#define SIN_LUT_SIZE 402

// Sin look-up table using doubles and floats at integer steps of "radians * 64"
static double sinTable_double[SIN_LUT_SIZE];
static float sinTable_float[SIN_LUT_SIZE];
static q15_16_t sinTable_fixedPoint[SIN_LUT_SIZE];

void init_sinLUT(void)
{
	for( int idx = 0; idx < SIN_LUT_SIZE; idx++)
	{
		double radians = ( (double) idx ) / 64.0;
		double output_double = sin( radians );
		sinTable_double[idx] = output_double;
		sinTable_float[idx] = (float) output_double;
		sinTable_fixedPoint[idx] = TOFIX(output_double, 16);
	}
}

double sin_LUT_double(double radians)
{
	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,402] (the size of our LUT). Round "radians" by adding 0.5 
	// and truncating. In this manner, all values between radian.0 and radian.49 become radian.5 to radian.99 and are truncated 
	// to radian (i.e. they are rounded down). However, all values between radian.5 and radian.99 become radian+1.0 to
	// radian+1.49, and are truncated to radian+1 (i.e. they are rounded up).
	//
	int idx = (int)( ( radians * 64 ) + 0.5 );

	// Ensure "idx" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" idx to a valid 
	// value instead of throwing an error.
	//
	while( idx >= SIN_LUT_SIZE ) idx -= SIN_LUT_SIZE;
	while( idx < 0 ) idx += SIN_LUT_SIZE;

	return sinTable_double[ idx ];
}

float sin_LUT_float(float radians)
{
	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,402] (the size of our LUT). Round "radians" by adding 0.5 
	// and truncating. In this manner, all values between radian.0 and radian.49 become radian.5 to radian.99 and are truncated 
	// to radian (i.e. they are rounded down). However, all values between radian.5 and radian.99 become radian+1.0 to
	// radian+1.49, and are truncated to radian+1 (i.e. they are rounded up).
	//
	int idx = (int)( ( radians * 64 ) + 0.5 );

	// Ensure "idx" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" idx to a valid 
	// value instead of throwing an error.
	//
	while( idx >= SIN_LUT_SIZE ) idx -= SIN_LUT_SIZE;
	while( idx < 0 ) idx += SIN_LUT_SIZE;

	return sinTable_float[ idx ];
}

q15_16_t sin_LUT_fixedPoint(q15_16_t radians)
{
	// Not accounting for negative numbers, overflow...
	return sinTable_fixedPoint[ ( radians & 0xFF8 ) >> 3 ];
}

double sin_LUT_double_interpolate(double degrees)
{
	//asdf
}

double sin_LUT_double_nonUniform(double degrees)
{
	//asdf
}


double sin_LUT_double_rangeReduction(double degrees)
{
	//asdf
}