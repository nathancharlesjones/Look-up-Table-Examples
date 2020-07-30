// Enforce -O0 optimization level (so that calls to sin and sin_LUT don't get optimized away)
//
#pragma GCC push_options
#pragma GCC optimize ("O0")

#include <math.h>
#include <stdlib.h>
#include "main.h"
#include "sin_lut.h"
#include "sincos.h"
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
//
volatile uint32_t testIterations = 1000;

int main(int argc, char * argv[])
{
	errno_t err = 0;

	p_systemTime_t p_start = systemTime_create();
	p_systemTime_t p_end = systemTime_create();

	sinLUT_implementation_t codeUnderTest[] = 
	{
		{ "Scaffolding",	fcn_scaffolding,	{.fcn_double = NULL}, 								0, 0, 0, 0, 0, 0, 0 },
		{ "Library Sin",	fcn_dbl_in_dbl_out,	{.fcn_double = sin}, 								0, 0, 0, 0, 0, 0, 0 },
		{ "LUT Double",		fcn_dbl_in_dbl_out,	{.fcn_double = sin_LUT_double},						0, 0, 0, 0, 0, 0, 0 },
		{ "LUT Float",		fcn_flt_in_flt_out,	{.fcn_float = sin_LUT_float},						0, 0, 0, 0, 0, 0, 0 },
		{ "LUT Fxd Pt",		fcn_fxd_in_fxd_out,	{.fcn_fixedPoint = sin_LUT_fixedPoint},				0, 0, 0, 0, 0, 0, 0 },
		{ "Dbl Interp",		fcn_dbl_in_dbl_out,	{.fcn_double = sin_LUT_double_interpolate},			0, 0, 0, 0, 0, 0, 0 },
		{ "Flt Interp",		fcn_flt_in_flt_out,	{.fcn_float = sin_LUT_float_interpolate},			0, 0, 0, 0, 0, 0, 0 },
		{ "Fxd Interp",		fcn_fxd_in_fxd_out,	{.fcn_fixedPoint = sin_LUT_fixedPoint_interpolate},	0, 0, 0, 0, 0, 0, 0 },
		{ "Dbl X/Y list",	fcn_dbl_in_dbl_out,	{.fcn_double = sin_LUT_double_nonUniform},			0, 0, 0, 0, 0, 0, 0 },
		{ "Flt X/Y list",	fcn_flt_in_flt_out,	{.fcn_float = sin_LUT_float_nonUniform},			0, 0, 0, 0, 0, 0, 0 },
		{ "Fxd X/Y list",	fcn_fxd_in_fxd_out,	{.fcn_fixedPoint = sin_LUT_fixedPoint_nonUniform},	0, 0, 0, 0, 0, 0, 0 },
		{ "Sin_32\t",		fcn_flt_in_flt_out,	{.fcn_float = sin_32},								0, 0, 0, 0, 0, 0, 0 },
		{ "Sin_52\t",		fcn_flt_in_flt_out,	{.fcn_float = sin_52},								0, 0, 0, 0, 0, 0, 0 },
		{ "Sin_73\t",		fcn_dbl_in_dbl_out,	{.fcn_double = sin_73},								0, 0, 0, 0, 0, 0, 0 },
		{ "Sin_121\t",		fcn_dbl_in_dbl_out,	{.fcn_double = sin_121},							0, 0, 0, 0, 0, 0, 0 },
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
			// Generate inputs for all three types of functions (double, float, fixed-point)
			//
			double output_CUT, input_double = (double) rand() / (double) RAND_MAX * 2.0 * PI;
			float output_float, input_float = (float) input_double;
			q15_16_t output_fixedPoint, input_fixedPoint = TOFIX(input_double, 16);

			// Call the correct function with the correct input data type based on the fcnSignature_t enum
			//
			switch( codeUnderTest[idx_CUT].function_enum )
			{
				case fcn_scaffolding:
					err = getSystemTime(p_start);
					ASSERT( err == 0 );
					output_CUT = 0;
					err = getSystemTime(p_end);
					ASSERT( err == 0 );
				break;

				case fcn_dbl_in_dbl_out:
					err = getSystemTime(p_start);
					ASSERT( err == 0 );
					output_CUT = codeUnderTest[idx_CUT].fcn_double( input_double );
					err = getSystemTime(p_end);
					ASSERT( err == 0 );
				break;

				case fcn_flt_in_flt_out:
					err = getSystemTime(p_start);
					ASSERT( err == 0 );
					output_float = codeUnderTest[idx_CUT].fcn_float( input_float );
					err = getSystemTime(p_end);
					ASSERT( err == 0 );
					output_CUT = (double) output_float;
				break;

				case fcn_fxd_in_fxd_out:
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
			
			// Add most recent computation time to the running total
			//
			codeUnderTest[idx_CUT].executionTime_ns += systemTimeDiff_ns(p_start, p_end);

			// Add current absolute error to the running total
			//
			double output_sin = sin( input_double );
			double absoluteError = fabs( output_sin - output_CUT );
			codeUnderTest[idx_CUT].absoluteError_sum += absoluteError;
			if( absoluteError > codeUnderTest[idx_CUT].absoluteError_max ) codeUnderTest[idx_CUT].absoluteError_max = absoluteError;

			// Add current percent error to the running total
			//
			double percentError;
			double expected = fabs( output_sin );
			if ( expected != 0 ) percentError = absoluteError / expected * 100.0;
			else percentError = 100.0;
			codeUnderTest[idx_CUT].percentError_sum += percentError;
		}

		// Determine averages
		//
		codeUnderTest[idx_CUT].executionTime_ns_avg = codeUnderTest[idx_CUT].executionTime_ns / testIterations;
		codeUnderTest[idx_CUT].absoluteError_avg = codeUnderTest[idx_CUT].absoluteError_sum / testIterations;
		codeUnderTest[idx_CUT].percentError_avg = codeUnderTest[idx_CUT].percentError_sum / testIterations;

		idx_CUT++;
	}

	printResults_CUT(testIterations, codeUnderTest);

	return EXIT_SUCCESS;
}

#pragma GCC pop_options