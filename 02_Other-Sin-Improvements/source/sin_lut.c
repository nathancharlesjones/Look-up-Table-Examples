#include <math.h>
#include <stdio.h>
#include "sin_lut.h"

#define SIN_LUT_SIZE 402
#define LAST_ELEMENT ( SIN_LUT_SIZE - 1 )

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
	// Convert "radians" from having 16 fractional bits to only having 6 fractional bits. The purpose of this is to get the lower
	// 9 bits of "radians_shifted" to align with the array indices (i.e. aligning the input to have 6 fractional bits is equivalent
	// to multiplying the floating-point numbers by 64 (2^6) above).
	//
	q25_6_t radians_shifted = FCONV(radians, 16, 6);

	// Ensure "radians_shifted" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" 
	// radians_shifted to a valid value instead of throwing an error.
	//
	while( radians_shifted >= SIN_LUT_SIZE ) radians_shifted -= SIN_LUT_SIZE;
	while( radians_shifted < 0 ) radians_shifted += SIN_LUT_SIZE;

	return sinTable_fixedPoint[ radians_shifted ];
}

double sin_LUT_double_interpolate(double radians)
{
	double ret;
	int x0, x1;

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,402] (the size of our LUT).
	//
	double x = radians * 64;

	// Ensure "x" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" x to a valid 
	// value instead of throwing an error.
	//
	while( x >= SIN_LUT_SIZE ) x -= SIN_LUT_SIZE;
	while( x < 0 ) x += SIN_LUT_SIZE;

	// First, check if our input value is greater than our last index (if this is the case, then we need to adjust our
	// x0 and x1 values accordingly, so we don't accidentally reference past the end of our LUT).
	//
	if ( x > LAST_ELEMENT )
	{
		x0 = LAST_ELEMENT - 1;
		x1 = LAST_ELEMENT; 
	}
	else
	{
		x0 = (int)( x );
		x1 = x0 + 1;
	}

	// Next, compute the linear interpolation. The slope is merely the difference between the table values, since the difference
	// in x is always 1. The "span" is the difference between our input and the lower table point, x0. The "offset" is the 
	// amount our y-value changes as a result of our span. The final value is equal to the y-value of the lower point plus this
	// offset. This algorithm constitutes one floating-point multiply and 3 floating-point additions/subtractions, and so is
	// relatively fast.
	//
	double y0 = sinTable_double[ x0 ];
	double slope = sinTable_double[ x1 ] - y0;
	double span = x - (double)( x0 );
	double offset = slope * span;
	ret = y0 + offset;

	return ret;
}

double sin_LUT_double_nonUniform(double degrees)
{
	//asdf
}

float sin_LUT_float_interpolate(float radians)
{
	float ret;
	int x0, x1;

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,402] (the size of our LUT).
	//
	float x = radians * 64;

	// Ensure "x" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" x to a valid 
	// value instead of throwing an error.
	//
	while( x >= SIN_LUT_SIZE ) x -= SIN_LUT_SIZE;
	while( x < 0 ) x += SIN_LUT_SIZE;

	// First, check if our input value is greater than our last index (if this is the case, then we need to adjust our
	// x0 and x1 values accordingly, so we don't accidentally reference past the end of our LUT).
	//
	if ( x > LAST_ELEMENT )
	{
		x0 = LAST_ELEMENT - 1;
		x1 = LAST_ELEMENT; 
	}
	else
	{
		x0 = (int)( x );
		x1 = x0 + 1;
	}

	// Next, compute the linear interpolation. The slope is merely the difference between the table values, since the difference
	// in x is always 1. The "span" is the difference between our input and the lower table point, x0. The "offset" is the 
	// amount our y-value changes as a result of our span. The final value is equal to the y-value of the lower point plus this
	// offset. This algorithm constitutes one floating-point multiply and 3 floating-point additions/subtractions, and so is
	// relatively fast.
	//
	float y0 = sinTable_float[ x0 ];
	float slope = sinTable_float[ x1 ] - y0;
	float span = x - (float)( x0 );
	float offset = slope * span;
	ret = y0 + offset;

	return ret;
}

float sin_LUT_float_nonUniform(float degrees)
{
	//asdf
}

q15_16_t sin_LUT_fixedPoint_interpolate(q15_16_t radians)
{
	q15_16_t ret, tableSize = FCONV(SIN_LUT_SIZE, 0, 16);
	int x0, x1;

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,402] (the size of our LUT).
	//
	q15_16_t x = FMULI( radians, 64 );

	// Ensure "x" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" x to a valid 
	// value instead of throwing an error. Uses "tableSize" instead of SIN_LUT_SIZE in order to match the radix position
	// for the fixed-point value "x" (which is in q15_16 format).
	//
	while( x >= SIN_LUT_SIZE ) x = FSUB( x, tableSize );
	while( x < 0 ) x = FADD( x, tableSize );

	// First, check if our input value is equal to the last element in our table (if this is the case, then we need to 
	// adjust our x0 and x1 values accordingly, so we don't accidentally reference past the end of our LUT). Otherwise,
	// x0, the lower index, is just equal to our truncated input value and the x1, the upper index, is one greater than
	// that.
	//
	if ( x == LAST_ELEMENT )
	{
		x0 = LAST_ELEMENT - 1;
		x1 = LAST_ELEMENT; 
	}
	else
	{
		x0 = FCONV( x, 16, 0 );
		x1 = x0 + 1;
	}

	// Next, compute the linear interpolation. The slope is merely the difference between the table values, since the difference
	// in x is always 1. The "span" is the difference between our input and the lower table point, x0. The "offset" is the 
	// amount our y-value changes as a result of our span. The final value is equal to the y-value of the lower point plus this
	// offset. This algorithm constitutes one floating-point multiply and 3 floating-point additions/subtractions, and so is
	// relatively fast.
	//
	q15_16_t y0 = sinTable_fixedPoint[ x0 ];
	q15_16_t slope = FSUB( sinTable_fixedPoint[ x1 ], y0 );
	q15_16_t span = FSUBG( x, x0, 16, 0, 16 );
	q15_16_t offset = FMUL( slope, span, 16 );
	ret = FADD( y0, offset );

	return ret;
}

q15_16_t sin_LUT_fixedPoint_nonUniform(q15_16_t degrees)
{
	//asdf
}

double sin_LUT_double_rangeReduction(double degrees)
{
	//asdf
}

double sin_LUT_float_rangeReduction(double degrees)
{
	//asdf
}

double sin_LUT_fixedPoint_rangeReduction(double degrees)
{
	//asdf
}