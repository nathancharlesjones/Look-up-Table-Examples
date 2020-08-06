#include <math.h>
#include <stdio.h>
#include "sin_lut.h"
#include "error.h"
#include "assert.h"

#define SIN_LUT_SIZE 404
#define LAST_ELEMENT ( SIN_LUT_SIZE - 1 )

// Sin look-up table using doubles and floats at integer steps of "radians * 64"
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

double sin_LUT_double(double radians)
{
	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians 
	// to a valid  value instead of throwing an error.
	//
	while( radians >= TWO_PI ) radians -= TWO_PI;
	while( radians < 0 ) radians += TWO_PI;
	

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT). Round "radians" by adding 0.5 
	// and truncating. In this manner, all values between radian.0 and radian.49 become radian.5 to radian.99 and are truncated 
	// to radian (i.e. they are rounded down). However, all values between radian.5 and radian.99 become radian+1.0 to
	// radian+1.49, and are truncated to radian+1 (i.e. they are rounded up).
	//
	int idx = (int)( ( radians * 64 ) + 0.5 );
	
	return sinTable_double[ idx ];
}

float sin_LUT_float(float radians)
{
	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians 
	// to a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI ) radians -= TWO_PI;
	while( radians < 0 ) radians += TWO_PI;

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT). Round "radians" by adding 0.5 
	// and truncating. In this manner, all values between radian.0 and radian.49 become radian.5 to radian.99 and are truncated 
	// to radian (i.e. they are rounded down). However, all values between radian.5 and radian.99 become radian+1.0 to
	// radian+1.49, and are truncated to radian+1 (i.e. they are rounded up).
	//
	int idx = (int)( ( radians * 64 ) + 0.5 );

	return sinTable_float[ idx ];
}

q0_31_t sin_LUT_fixedPoint_safe(q9_22_t radians)
{
	errno_t err = 0;
	q9_22_t index = 0;
	q9_22_t index_plus_half = 0;
	q9_22_t TWO_PI_FXD = TOFIX( TWO_PI, 28 );
	int rounded_index = 0;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians to 
	// a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI_FXD )
	{
		err = SAFE_FSUB( radians, TWO_PI_FXD, &radians );
		ASSERT( err == 0 );
	}
	while( radians < 0 )
	{
		err = SAFE_FADD( radians, TWO_PI_FXD, &radians );
		ASSERT( err == 0 );
	}

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT). Add 0.5 and then convert
	// to int (i.e. "q31_0" format) in order to implement a basic rounding.
	//
	err = SAFE_FMULI( radians, 64, &index );
	ASSERT( err == 0 );

	err = SAFE_FADD( index, TOFIX( 0.5, 22 ), &index_plus_half  );
	ASSERT( err == 0 );

	rounded_index = FCONV( index_plus_half, 22, 0 );

	return sinTable_fixedPoint[ rounded_index ];
}

q0_31_t sin_LUT_fixedPoint(q9_22_t radians)
{
	q9_22_t index = 0;
	q9_22_t index_plus_half = 0;
	q9_22_t TWO_PI_FXD = TOFIX( TWO_PI, 28 );
	int rounded_index = 0;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians to 
	// a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI_FXD ) radians = FSUB( radians, TWO_PI_FXD );
	while( radians < 0 ) radians = FADD(radians, TWO_PI_FXD );

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT). Add 0.5 and then convert
	// to int (i.e. "q31_0" format) in order to implement a basic rounding.
	//
	index = FMULI( radians, 64 );
	index_plus_half = FADD( index, TOFIX( 0.5, 22 ) );
	rounded_index = FCONV( index_plus_half, 22, 0 );

	return sinTable_fixedPoint[ rounded_index ];
}

double sin_LUT_double_interpolate(double radians)
{
	double ret;
	int x0, x1;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians 
	// to a valid  value instead of throwing an error.
	//
	while( radians >= TWO_PI ) radians -= TWO_PI;
	while( radians < 0 ) radians += TWO_PI;

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT).
	//
	double x = radians * 64;

	// Get the indices for the array elements that are just below and just above "x" by type-casting to int (to get x0, the 
	// below element) and then adding one (to get x1, the above element). We can safely do this without any bounds checking
	// since, in the first step of this function, we ensured that our input was less than 2*PI and 2*PI*64 is only a litle
	// over 402.12; in this manner, the most x0 and x1 will ever be is 402 and 403, which are still within the bounds of our
	// array.
	//
	x0 = (int)( x );
	x1 = x0 + 1;

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

typedef struct point_double_t
{
	double x;
	double y;
} point_double_t;

typedef struct point_float_t
{
	float x;
	float y;
} point_float_t;

typedef struct point_fixed_t
{
	q9_22_t x;
	q0_31_t y;
} point_fixed_t;

#define NONUNIFORM_0DOT007ERROR_SIZE 25
#define LAST_ELEMENT_0DOT007ERROR ( NONUNIFORM_0DOT007ERROR_SIZE - 1 )

static point_double_t nonUniform_double_0dot007error[NONUNIFORM_0DOT007ERROR_SIZE] = 
{
	{ 0.000000000000000000,  0.000000000000000000 },
	{ 0.246907827863357000,	 0.244406737256656000 },
	{ 0.569239513635344000,	 0.538991638538422000 },
	{ 0.843093840424931000,  0.746704576346488000 },
	{ 1.094141848997940000,	 0.888534848533041000 },
	{ 1.334153179182300000,	 0.972130433247043000 },
	{ 1.570796370506290000,	 0.999999999999999000 },
	{ 1.807439561830270000,	 0.972130412751585000 },
	{ 2.047450894544720000,	 0.888534807261842000 },
	{ 2.298498908948140000,	 0.746704512635679000 },
	{ 2.572353247420710000,	 0.538991548018389000 },
	{ 2.894684960259430000,	 0.244406606803670000 },
	{ 3.141592653589790000,	 0.000000000000000122 },
	{ 3.388500615986130000,	-0.244406867709622000 },
	{ 3.710832274691400000,	-0.538991729058444000 },
	{ 3.984686589798000000,	-0.746704640057288000 },
	{ 4.235734592540600000,	-0.888534889804232000 },
	{ 4.475745920194870000,	-0.972130453742493000 },
	{ 4.712388980384690000,	-1.000000000000000000 },
	{ 4.949032302842850000,	-0.972130392256120000 },
	{ 5.189043638087370000,	-0.888534765990635000 },
	{ 5.440091658321210000,	-0.746704448924861000 },
	{ 5.713946008476780000,	-0.538991457498343000 },
	{ 6.036277748382230000,	-0.244406476350662000 },
	{ 6.283185307179590000,  0.000000000000000000 }
};

static point_float_t nonUniform_float_0dot007error[NONUNIFORM_0DOT007ERROR_SIZE] = 
{
	{ 0.000000000000000000,  0.000000000000000000 },
	{ 0.246907827863357000,	 0.244406737256656000 },
	{ 0.569239513635344000,	 0.538991638538422000 },
	{ 0.843093840424931000,  0.746704576346488000 },
	{ 1.094141848997940000,	 0.888534848533041000 },
	{ 1.334153179182300000,	 0.972130433247043000 },
	{ 1.570796370506290000,	 0.999999999999999000 },
	{ 1.807439561830270000,	 0.972130412751585000 },
	{ 2.047450894544720000,	 0.888534807261842000 },
	{ 2.298498908948140000,	 0.746704512635679000 },
	{ 2.572353247420710000,	 0.538991548018389000 },
	{ 2.894684960259430000,	 0.244406606803670000 },
	{ 3.141592653589790000,	 0.000000000000000122 },
	{ 3.388500615986130000,	-0.244406867709622000 },
	{ 3.710832274691400000,	-0.538991729058444000 },
	{ 3.984686589798000000,	-0.746704640057288000 },
	{ 4.235734592540600000,	-0.888534889804232000 },
	{ 4.475745920194870000,	-0.972130453742493000 },
	{ 4.712388980384690000,	-1.000000000000000000 },
	{ 4.949032302842850000,	-0.972130392256120000 },
	{ 5.189043638087370000,	-0.888534765990635000 },
	{ 5.440091658321210000,	-0.746704448924861000 },
	{ 5.713946008476780000,	-0.538991457498343000 },
	{ 6.036277748382230000,	-0.244406476350662000 },
	{ 6.283185307179590000,  0.000000000000000000 }
};

static point_fixed_t nonUniform_fixed_0dot007error[NONUNIFORM_0DOT007ERROR_SIZE] = 
{
	{ TOFIX(0.000000000000000000, 22),  TOFIX(0.000000000000000000, 31) },
	{ TOFIX(0.246907827863357000, 22),	TOFIX(0.244406737256656000, 31) },
	{ TOFIX(0.569239513635344000, 22),	TOFIX(0.538991638538422000, 31) },
	{ TOFIX(0.843093840424931000, 22),  TOFIX(0.746704576346488000, 31) },
	{ TOFIX(1.094141848997940000, 22),	TOFIX(0.888534848533041000, 31) },
	{ TOFIX(1.334153179182300000, 22),	TOFIX(0.972130433247043000, 31) },
	{ TOFIX(1.570796370506290000, 22),	TOFIX(0.999999999999999000, 31) },
	{ TOFIX(1.807439561830270000, 22),	TOFIX(0.972130412751585000, 31) },
	{ TOFIX(2.047450894544720000, 22),	TOFIX(0.888534807261842000, 31) },
	{ TOFIX(2.298498908948140000, 22),	TOFIX(0.746704512635679000, 31) },
	{ TOFIX(2.572353247420710000, 22),	TOFIX(0.538991548018389000, 31) },
	{ TOFIX(2.894684960259430000, 22),	TOFIX(0.244406606803670000, 31) },
	{ TOFIX(3.141592653589790000, 22),	TOFIX(0.000000000000000122, 31) },
	{ TOFIX(3.388500615986130000, 22),	TOFIX(-0.244406867709622000, 31) },
	{ TOFIX(3.710832274691400000, 22),	TOFIX(-0.538991729058444000, 31) },
	{ TOFIX(3.984686589798000000, 22),	TOFIX(-0.746704640057288000, 31) },
	{ TOFIX(4.235734592540600000, 22),	TOFIX(-0.888534889804232000, 31) },
	{ TOFIX(4.475745920194870000, 22),	TOFIX(-0.972130453742493000, 31) },
	{ TOFIX(4.712388980384690000, 22),	TOFIX(-1.000000000000000000, 31) },
	{ TOFIX(4.949032302842850000, 22),	TOFIX(-0.972130392256120000, 31) },
	{ TOFIX(5.189043638087370000, 22),	TOFIX(-0.888534765990635000, 31) },
	{ TOFIX(5.440091658321210000, 22),	TOFIX(-0.746704448924861000, 31) },
	{ TOFIX(5.713946008476780000, 22),	TOFIX(-0.538991457498343000, 31) },
	{ TOFIX(6.036277748382230000, 22),	TOFIX(-0.244406476350662000, 31) },
	{ TOFIX(6.283185307179590000, 22),  TOFIX(0.000000000000000000, 31) }
};

double sin_LUT_double_nonUniform(double radians)
{
	double ret;
	int low = 0;
	int high = LAST_ELEMENT_0DOT007ERROR;
	int mid = ( low + high ) / 2;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians 
	// to a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI ) radians -= TWO_PI;
	while( radians < 0 ) radians += TWO_PI;

	// Perform a binary search to find the LUT elements that will be used to interpolate the final value. Returns the last 
	// list element if "radians" is greater than the last element.
	//
	while( low <= high )
	{
		mid = ( low + high ) / 2;
		if( ( radians > nonUniform_double_0dot007error[mid].x ) && ( radians < nonUniform_double_0dot007error[mid+1].x ) ) break;
		if( radians < nonUniform_double_0dot007error[mid].x ) high = mid;
		else low = mid;
	}

	// Set "ret" equal to the y value of the last table element if "mid" came out to be the last element. Otherwise, compute
	// the linear interpolation.
	//
	if( mid == LAST_ELEMENT_0DOT007ERROR ) ret = nonUniform_double_0dot007error[mid].y;
	else
	{
		// Compute the linear interpolation. The slope is the standard "rise" over "run". The "span" is the difference 
		// between our input and the lower table point, x0. The "offset" is the amount our y-value changes as a result of our 
		// span. The final value is equal to the y-value of the lower point plus this offset.
		//
		double x0 = nonUniform_double_0dot007error[mid].x;
		double y0 = nonUniform_double_0dot007error[mid].y;
		double rise = nonUniform_double_0dot007error[mid+1].y - y0;
		double run = nonUniform_double_0dot007error[mid+1].x - x0;
		double slope = rise / run;
		double span = radians - x0;
		double offset = slope * span;
		ret = y0 + offset;
	}

	return ret;
}

float sin_LUT_float_interpolate(float radians)
{
	float ret;
	int x0, x1;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians 
	// to a valid  value instead of throwing an error.
	//
	while( radians >= TWO_PI ) radians -= TWO_PI;
	while( radians < 0 ) radians += TWO_PI;

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT).
	//
	float x = radians * 64;

	// Get the indices for the array elements that are just below and just above "x" by type-casting to int (to get x0, the 
	// below element) and then adding one (to get x1, the above element). We can safely do this without any bounds checking
	// since, in the first step of this function, we ensured that our input was less than 2*PI and 2*PI*64 is only a litle
	// over 402.12; in this manner, the most x0 and x1 will ever be is 402 and 403, which are still within the bounds of our
	// array.
	//
	x0 = (int)( x );
	x1 = x0 + 1;

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

float sin_LUT_float_nonUniform(float radians)
{
	float ret;
	int low = 0;
	int high = LAST_ELEMENT_0DOT007ERROR;
	int mid = ( low + high ) / 2;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians 
	// to a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI ) radians -= TWO_PI;
	while( radians < 0 ) radians += TWO_PI;

	// Perform a binary search to find the LUT elements that will be used to interpolate the final value. Returns the last 
	// list element if "radians" is greater than the last element.
	//
	while( low <= high )
	{
		mid = ( low + high ) / 2;
		if( ( radians > nonUniform_float_0dot007error[mid].x ) && ( radians < nonUniform_float_0dot007error[mid+1].x ) ) break;
		if( radians < nonUniform_float_0dot007error[mid].x ) high = mid;
		else low = mid;
	}

	// Set "ret" equal to the y value of the last table element if "mid" came out to be the last element. Otherwise, compute
	// the linear interpolation.
	//
	if( mid == LAST_ELEMENT_0DOT007ERROR ) ret = nonUniform_float_0dot007error[mid].y;
	else
	{
		// Compute the linear interpolation. The slope is the standard "rise" over "run". The "span" is the difference 
		// between our input and the lower table point, x0. The "offset" is the amount our y-value changes as a result of our 
		// span. The final value is equal to the y-value of the lower point plus this offset.
		//
		float x0 = nonUniform_float_0dot007error[mid].x;
		float y0 = nonUniform_float_0dot007error[mid].y;
		float rise = nonUniform_float_0dot007error[mid+1].y - y0;
		float run = nonUniform_float_0dot007error[mid+1].x - x0;
		float slope = rise / run;
		float span = radians - x0;
		float offset = slope * span;
		ret = y0 + offset;
	}

	return ret;
}

q0_31_t sin_LUT_fixedPoint_interpolate_safe(q9_22_t radians)
{
	errno_t err = 0;
	q0_31_t y0 = 0;
	q0_31_t slope = 0;
	q0_31_t offset = 0;
	q0_31_t ret = 0;
	q9_22_t x = 0;
	q9_22_t span = 0;
	q9_22_t TWO_PI_FXD = TOFIX(TWO_PI, 22);
	int x0, x1;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians to 
	// a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI_FXD )
	{
		err = SAFE_FSUB( radians, TWO_PI_FXD, &radians );
		ASSERT( err == 0 );
	}
	while( radians < 0 )
	{
		err = SAFE_FADD( radians, TWO_PI_FXD, &radians );
		ASSERT( err == 0 );
	}

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT).
	//
	err = SAFE_FMULI( radians, 64, &x );
	ASSERT( err == 0 );

	// Get the indices for the array elements that are just below and just above "x" by converting from q16 to q0 (to get x0, 
	// the below element) and then adding one (to get x1, the above element). We can safely do this without any bounds checking
	// since, in the first step of this function, we ensured that our input was less than 2*PI and 2*PI*64 is only a litle
	// over 402.12; in this manner, the most x0 and x1 will ever be is 402 and 403, which are still within the bounds of our
	// array.
	//
	x0 = FCONV( x, 22, 0 );
	x1 = x0 + 1;

	// Next, compute the linear interpolation. The slope is merely the difference between the table values, since the difference
	// in x is always 1. The "span" is the difference between our input and the lower table point, x0. The "offset" is the 
	// amount our y-value changes as a result of our span. The final value is equal to the y-value of the lower point plus this
	// offset. This algorithm constitutes one floating-point multiply and 3 floating-point additions/subtractions, and so is
	// relatively fast.
	//
	y0 = sinTable_fixedPoint[ x0 ];

	// NOTE: Since "slope" is in the output format of q0.31, this variable is ONLY valid in the range [-1,1] and WILL NOT hold
	// the correct value otherwise.
	//
	err = SAFE_FSUB( sinTable_fixedPoint[ x1 ], y0, &slope );
	ASSERT( err == 0 );

	err = SAFE_FSUBG( x, x0, 22, 0, 22, &span );
	ASSERT( err == 0 );
	
	err = SAFE_FMULG( slope, span, 31, 22, 31, &offset );
	ASSERT( err == 0 );

	err = SAFE_FADD( y0, offset, &ret );
	ASSERT( err == 0 );

	return ret;
}

q0_31_t sin_LUT_fixedPoint_interpolate(q9_22_t radians)
{
	q9_22_t TWO_PI_FXD = TOFIX(TWO_PI, 22);

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians to 
	// a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI_FXD ) radians = FSUB( radians, TWO_PI_FXD );
	while( radians < 0 ) radians = FADD( radians, TWO_PI_FXD );

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT).
	//
	q9_22_t x = FMULI( radians, 64 );

	// Get the indices for the array elements that are just below and just above "x" by converting from q16 to q0 (to get x0, 
	// the below element) and then adding one (to get x1, the above element). We can safely do this without any bounds checking
	// since, in the first step of this function, we ensured that our input was less than 2*PI and 2*PI*64 is only a litle
	// over 402.12; in this manner, the most x0 and x1 will ever be is 402 and 403, which are still within the bounds of our
	// array.
	//
	int x0 = FCONV( x, 22, 0 );
	int x1 = x0 + 1;

	// Next, compute the linear interpolation. The slope is merely the difference between the table values, since the difference
	// in x is always 1. The "span" is the difference between our input and the lower table point, x0. The "offset" is the 
	// amount our y-value changes as a result of our span. The final value is equal to the y-value of the lower point plus this
	// offset. This algorithm constitutes one floating-point multiply and 3 floating-point additions/subtractions, and so is
	// relatively fast.
	//
	q0_31_t y0 = sinTable_fixedPoint[ x0 ];

	// NOTE: Since "slope" is in the output format of q0.31, this variable is ONLY valid in the range [-1,1] and WILL NOT hold
	// the correct value otherwise.
	//
	q0_31_t slope = FSUB( sinTable_fixedPoint[ x1 ], y0 );
	q9_22_t span = FSUBG( x, x0, 22, 0, 22 );
	q0_31_t offset = FMULG( (int64_t)slope, (int64_t)span, 31, 22, 31 );
	q0_31_t ret = FADD( y0, offset );

	return ret;
}

q0_31_t sin_LUT_fixedPoint_nonUniform_safe(q9_22_t radians)
{
	// The input doesn't necessarily need to be in q9_22 format, since the x-values never go over 2*PI. However, I'm using it here
	// to avoid needing to rewrite the rest of the test code to accept a fourth function signature (the first two fixed-point LUTs
	// already require an input parameter of at least q9_22).

	errno_t err = 0;
	q9_22_t x0 = 0;
	q9_22_t run = 0;
	q9_22_t span = 0;
	q0_31_t y0 = 0;
	q0_31_t rise = 0;
	q0_31_t slope = 0;
	q0_31_t offset = 0;
	q0_31_t ret = 0;
	q9_22_t TWO_PI_FXD = TOFIX(TWO_PI, 28);
	int low = 0;
	int high = LAST_ELEMENT_0DOT007ERROR;
	int mid = ( low + high ) / 2;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians 
	// to a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI_FXD )
	{
		err = SAFE_FSUB( radians, TWO_PI_FXD, &radians );
		ASSERT( err == 0 );
	}
	while( radians < 0 )
	{
		err = SAFE_FADD( radians, TWO_PI_FXD, &radians );
		ASSERT( err == 0 );
	}

	// Perform a binary search to find the LUT elements that will be used to interpolate the final value. Returns the last 
	// list element if "radians" is greater than the last element.
	//
	while( low <= high )
	{
		mid = ( low + high ) / 2;
		if( ( radians > nonUniform_fixed_0dot007error[mid].x ) && ( radians < nonUniform_fixed_0dot007error[mid+1].x ) ) break;
		if( radians < nonUniform_fixed_0dot007error[mid].x ) high = mid;
		else low = mid;
	}

	// Set "ret" equal to the y value of the last table element if "mid" came out to be the last element. Otherwise, compute
	// the linear interpolation.
	//
	if( mid == LAST_ELEMENT_0DOT007ERROR ) ret = nonUniform_fixed_0dot007error[mid].y;
	else
	{
		// Compute the linear interpolation. The slope is the standard "rise" over "run". The "span" is the difference 
		// between our input and the lower table point, x0. The "offset" is the amount our y-value changes as a result of our 
		// span. The final value is equal to the y-value of the lower point plus this offset.
		//
		x0 = nonUniform_fixed_0dot007error[mid].x;
		y0 = nonUniform_fixed_0dot007error[mid].y;

		err = SAFE_FSUB( nonUniform_fixed_0dot007error[mid+1].y, y0, &rise );
		ASSERT( err == 0 );

		err = SAFE_FSUB( nonUniform_fixed_0dot007error[mid+1].x, x0, &run );
		ASSERT( err == 0 );

		err = SAFE_FDIVG( rise, run, 31, 22, 31, &slope );
		ASSERT( err == 0 );

		err = SAFE_FSUB( radians, x0, &span );
		ASSERT( err == 0 );

		err = SAFE_FMULG( slope, span, 31, 22, 31, &offset );
		ASSERT( err == 0 );

		err = SAFE_FADD( y0, offset, &ret );
		ASSERT( err == 0 );
	}

	return ret;
}

q0_31_t sin_LUT_fixedPoint_nonUniform(q9_22_t radians)
{
	// The input doesn't necessarily need to be in q9_22 format, since the x-values never go over 2*PI. However, I'm using it here
	// to avoid needing to rewrite the rest of the test code to accept a fourth function signature (the first two fixed-point LUTs
	// already require an input parameter of at least q9_22).

	q0_31_t ret;
	q9_22_t TWO_PI_FXD = TOFIX(TWO_PI, 28);
	int low = 0;
	int high = LAST_ELEMENT_0DOT007ERROR;
	int mid = ( low + high ) / 2;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians 
	// to a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI_FXD ) radians = FSUB( radians, TWO_PI_FXD );
	while( radians < 0 ) radians = FADD( radians, TWO_PI_FXD );

	// Perform a binary search to find the LUT elements that will be used to interpolate the final value. Returns the last 
	// list element if "radians" is greater than the last element.
	//
	while( low <= high )
	{
		mid = ( low + high ) / 2;
		if( ( radians > nonUniform_fixed_0dot007error[mid].x ) && ( radians < nonUniform_fixed_0dot007error[mid+1].x ) ) break;
		if( radians < nonUniform_fixed_0dot007error[mid].x ) high = mid - 1;
		else low = mid + 1;
	}

	// Set "ret" equal to the y value of the last table element if "mid" came out to be the last element. Otherwise, compute
	// the linear interpolation.
	//
	if( mid == LAST_ELEMENT_0DOT007ERROR ) ret = nonUniform_fixed_0dot007error[mid].y;
	else
	{
		// Compute the linear interpolation. The slope is the standard "rise" over "run". The "span" is the difference 
		// between our input and the lower table point, x0. The "offset" is the amount our y-value changes as a result of our 
		// span. The final value is equal to the y-value of the lower point plus this offset.
		//
		q9_22_t x0 = nonUniform_fixed_0dot007error[mid].x;
		q0_31_t y0 = nonUniform_fixed_0dot007error[mid].y;

		q0_31_t rise = FSUB( nonUniform_fixed_0dot007error[mid+1].y, y0 );
		q9_22_t run = FSUB( nonUniform_fixed_0dot007error[mid+1].x, x0 );
		q0_31_t slope = FDIVG( (int64_t)rise, (int64_t)run, 31, 22, 31 );
		q9_22_t span = FSUB( radians, x0 );
		q0_31_t offset = FMULG( (int64_t)slope, (int64_t)span, 31, 22, 31 );
		ret = FADD( y0, offset );
	}

	return ret;
}