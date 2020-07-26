// Enforce -O0 optimization level (so that calls to sin and sin_LUT don't get optimized away)
//
#pragma GCC push_options
#pragma GCC optimize ("O0")

#include <math.h>
#include <stdlib.h>
#include "main.h"
#include "sin_lut.h"
#include "hardwareAPI.h"

/*
Knobs:
- Input: float, int, fixed-point
- Output: float, fixed-point
- Linear interpolation
- Non-uniform distribution
- Range reduction
- Compare to polynomial approximations?
Progression:
- Make it faster
	- Change input/output to fixed-point
	- Take out linear interpolation/non-uniform distribution (but possibly add more points)
- Make it more accurate
	- Add linear interpolation
	- Non-uniform distribution
	- Add more points
	- Go from fixed-point -> floats -> doubles
- Make it smaller
	- Change to floats/fixed-point
	- Range reduction
	- Remove points (keep conversions in mind (i.e. powers-of-two multiplication); speed reductions could be significant if they aren't)
*/

// testIterations is marked as "volatile" so that it can be updated with a debugger; otherwise
// it could be const or #define.
volatile uint32_t testIterations = 1000;

int main(int argc, char * argv[])
{
	errno_t err = 0;

	p_systemTime_t p_start = systemTime_create();
	p_systemTime_t p_end = systemTime_create();

	sinLUT_implementation_t	scaffolding_struct = {					"Scaffolding",	fcn_scaffolding,				{.fcn_double = NULL}, 						0, 0, 0, 0, 0, 0 };
	sinLUT_implementation_t	sin_library_struct = {					"Library Sin",	fcn_sin,						{.fcn_double = sin}, 						0, 0, 0, 0, 0, 0 };
	sinLUT_implementation_t	sin_LUT_double_struct = {				"LUT Double",	fcn_sin_LUT_double,				{.fcn_double = sin_LUT_double},				0, 0, 0, 0, 0, 0 };
	sinLUT_implementation_t	sin_LUT_float_struct = {				"LUT Float",	fcn_sin_LUT_float,				{.fcn_float = sin_LUT_float},				0, 0, 0, 0, 0, 0 };
	sinLUT_implementation_t sin_LUT_fixedPoint_struct = {			"LUT Fxd Pt",	fcn_sin_LUT_fixedPoint,			{.fcn_fixedPoint = sin_LUT_fixedPoint},		0, 0, 0, 0, 0, 0 };
	sinLUT_implementation_t sin_LUT_double_interpolate_struct = {	"Dbl Interp",	fcn_sin_LUT_double_interpolate,	{.fcn_double = sin_LUT_double_interpolate},	0, 0, 0, 0, 0, 0 };
	sinLUT_implementation_t sin_LUT_float_interpolate_struct = {	"Flt Interp",	fcn_sin_LUT_float_interpolate,	{.fcn_float = sin_LUT_float_interpolate},	0, 0, 0, 0, 0, 0 };

	sinLUT_implementation_t codeUnderTest[] = 
	{
		scaffolding_struct,
		sin_library_struct,
		sin_LUT_double_struct,
		sin_LUT_float_struct,
		sin_LUT_fixedPoint_struct,
		sin_LUT_double_interpolate_struct,
		sin_LUT_float_interpolate_struct,
		{0}
	};

	// Check that p_start and p_end were allocated
	//
	ASSERT(p_start != NULL);
	ASSERT(p_end != NULL);
	
	// Initialize the underlying hardware (if necessary)
	//
	initHardware();
	init_sinLUT();

	// Seed the pseudo-random number generator
	//
	uint32_t time_ns;
	err = elapsedSystemTime_ns(&time_ns);
	ASSERT( err == 0 );
	srand((unsigned) time_ns);

	int idx_CUT = 0;
	while( codeUnderTest[idx_CUT].function_enum != NOT_ASSIGNED )
	{
		for( int idx_test = 0; idx_test < testIterations; idx_test++ )
		{
			double output_CUT, input_double = (double) rand() / (double) RAND_MAX * 2.0 * PI;
			float output_float, input_float = (float) input_double;
			q15_16_t output_fixedPoint, input_fixedPoint = TOFIX(input_double, 16);
			switch( codeUnderTest[idx_CUT].function_enum )
			{
				case fcn_scaffolding:
					err = getSystemTime(p_start);
					ASSERT( err == 0 );
					output_CUT = 0;
					err = getSystemTime(p_end);
					ASSERT( err == 0 );
				break;

				case fcn_sin:
				case fcn_sin_LUT_double:
				case fcn_sin_LUT_double_interpolate:
				case fcn_sin_LUT_double_nonUniform:
				case fcn_sin_LUT_double_rangeReduction:
					err = getSystemTime(p_start);
					ASSERT( err == 0 );
					output_CUT = codeUnderTest[idx_CUT].fcn_double( input_double);
					err = getSystemTime(p_end);
					ASSERT( err == 0 );
				break;

				case fcn_sin_LUT_float:
				case fcn_sin_LUT_float_interpolate:
					err = getSystemTime(p_start);
					ASSERT( err == 0 );
					output_float = codeUnderTest[idx_CUT].fcn_float( input_float );
					err = getSystemTime(p_end);
					ASSERT( err == 0 );
					output_CUT = (double) output_float;
				break;

				case fcn_sin_LUT_fixedPoint:
					err = getSystemTime(p_start);
					ASSERT( err == 0 );
					output_fixedPoint = codeUnderTest[idx_CUT].fcn_fixedPoint( input_fixedPoint );
					err = getSystemTime(p_end);
					ASSERT( err == 0 );
					output_CUT = TOFLT(output_fixedPoint, 16);
				break;

				default:
					ASSERT(0);
				// Unreachable
				break;
			}			
			
			codeUnderTest[idx_CUT].executionTime_ns += systemTimeDiff_ns(p_start, p_end);

			double output_sin = sin( input_double );
			double absoluteError = fabs( output_sin - output_CUT );
			codeUnderTest[idx_CUT].absoluteError_sum += absoluteError;

			double percentError;
			double expected = fabs( output_sin );
			if ( expected != 0 ) percentError = absoluteError / expected * 100.0;
			else percentError = 100.0;
			codeUnderTest[idx_CUT].percentError_sum += percentError;

		}

		codeUnderTest[idx_CUT].executionTime_ns_avg = codeUnderTest[idx_CUT].executionTime_ns / testIterations;
		codeUnderTest[idx_CUT].absoluteError_avg = codeUnderTest[idx_CUT].absoluteError_sum / testIterations;
		codeUnderTest[idx_CUT].percentError_avg = codeUnderTest[idx_CUT].percentError_sum / testIterations;

		idx_CUT++;
	}

	printResults_CUT(testIterations, codeUnderTest);

	return EXIT_SUCCESS;
}

#pragma GCC pop_options