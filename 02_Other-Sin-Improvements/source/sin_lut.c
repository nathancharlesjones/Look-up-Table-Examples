#include <math.h>
#include <stdio.h>
#include "sin_lut.h"

#define SIN_LUT_SIZE 404
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

q15_16_t sin_LUT_fixedPoint(q15_16_t radians)
{
	q15_16_t TWO_PI_FXD = TOFIX(TWO_PI, 16);

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians to 
	// a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI_FXD ) radians = FSUB( radians, TWO_PI_FXD );
	while( radians < 0 ) radians = FADD( radians, TWO_PI_FXD );

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT).
	//
	q15_16_t x = FMULI( radians, 64 );

	return sinTable_fixedPoint[ FCONV( x, 16, 0 ) ];
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
	q15_16_t x;
	q15_16_t y;
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
	{ TOFIX(0.000000000000000000, 16),  TOFIX(0.000000000000000000, 16) },
	{ TOFIX(0.246907827863357000, 16),	TOFIX(0.244406737256656000, 16) },
	{ TOFIX(0.569239513635344000, 16),	TOFIX(0.538991638538422000, 16) },
	{ TOFIX(0.843093840424931000, 16),  TOFIX(0.746704576346488000, 16) },
	{ TOFIX(1.094141848997940000, 16),	TOFIX(0.888534848533041000, 16) },
	{ TOFIX(1.334153179182300000, 16),	TOFIX(0.972130433247043000, 16) },
	{ TOFIX(1.570796370506290000, 16),	TOFIX(0.999999999999999000, 16) },
	{ TOFIX(1.807439561830270000, 16),	TOFIX(0.972130412751585000, 16) },
	{ TOFIX(2.047450894544720000, 16),	TOFIX(0.888534807261842000, 16) },
	{ TOFIX(2.298498908948140000, 16),	TOFIX(0.746704512635679000, 16) },
	{ TOFIX(2.572353247420710000, 16),	TOFIX(0.538991548018389000, 16) },
	{ TOFIX(2.894684960259430000, 16),	TOFIX(0.244406606803670000, 16) },
	{ TOFIX(3.141592653589790000, 16),	TOFIX(0.000000000000000122, 16) },
	{ TOFIX(3.388500615986130000, 16),	TOFIX(-0.244406867709622000, 16) },
	{ TOFIX(3.710832274691400000, 16),	TOFIX(-0.538991729058444000, 16) },
	{ TOFIX(3.984686589798000000, 16),	TOFIX(-0.746704640057288000, 16) },
	{ TOFIX(4.235734592540600000, 16),	TOFIX(-0.888534889804232000, 16) },
	{ TOFIX(4.475745920194870000, 16),	TOFIX(-0.972130453742493000, 16) },
	{ TOFIX(4.712388980384690000, 16),	TOFIX(-1.000000000000000000, 16) },
	{ TOFIX(4.949032302842850000, 16),	TOFIX(-0.972130392256120000, 16) },
	{ TOFIX(5.189043638087370000, 16),	TOFIX(-0.888534765990635000, 16) },
	{ TOFIX(5.440091658321210000, 16),	TOFIX(-0.746704448924861000, 16) },
	{ TOFIX(5.713946008476780000, 16),	TOFIX(-0.538991457498343000, 16) },
	{ TOFIX(6.036277748382230000, 16),	TOFIX(-0.244406476350662000, 16) },
	{ TOFIX(6.283185307179590000, 16),  TOFIX(0.000000000000000000, 16) }
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
		if( radians < nonUniform_double_0dot007error[mid].x ) high = mid - 1;
		else low = mid + 1;
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
		if( radians < nonUniform_float_0dot007error[mid].x ) high = mid - 1;
		else low = mid + 1;
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

q15_16_t sin_LUT_fixedPoint_interpolate(q15_16_t radians)
{
	q15_16_t ret, TWO_PI_FXD = TOFIX(TWO_PI, 16);
	int x0, x1;

	// Ensure "radians" is within a valid range. Takes advantage of the fact that sin is periodic to merely "wrap" radians to 
	// a valid value instead of throwing an error.
	//
	while( radians >= TWO_PI_FXD ) radians = FSUB( radians, TWO_PI_FXD );
	while( radians < 0 ) radians = FADD( radians, TWO_PI_FXD );

	// Multiply "radians" by 64 to map the range [0,2*PI] to the range [0,403] (the size of our LUT).
	//
	q15_16_t x = FMULI( radians, 64 );

	// Get the indices for the array elements that are just below and just above "x" by converting from q16 to q0 (to get x0, 
	// the below element) and then adding one (to get x1, the above element). We can safely do this without any bounds checking
	// since, in the first step of this function, we ensured that our input was less than 2*PI and 2*PI*64 is only a litle
	// over 402.12; in this manner, the most x0 and x1 will ever be is 402 and 403, which are still within the bounds of our
	// array.
	//
	x0 = FCONV( x, 16, 0 );
	x1 = x0 + 1;

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

q15_16_t sin_LUT_fixedPoint_nonUniform(q15_16_t radians)
{
	q15_16_t ret, TWO_PI_FXD = TOFIX(TWO_PI, 16);
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
		q15_16_t x0 = nonUniform_fixed_0dot007error[mid].x;
		q15_16_t y0 = nonUniform_fixed_0dot007error[mid].y;
		q15_16_t rise = FSUB( nonUniform_fixed_0dot007error[mid+1].y, y0 );
		q15_16_t run = FSUB( nonUniform_fixed_0dot007error[mid+1].x, x0 );
		q15_16_t slope = FDIV( rise, run, 16 );
		q15_16_t span = FSUB( radians, x0 );
		q15_16_t offset = FMUL( slope, span, 16 );
		ret = FADD( y0, offset );
	}

	return ret;
}