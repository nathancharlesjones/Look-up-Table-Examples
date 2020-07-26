// Enforce -O0 optimization level (so that calls to sin and sin_LUT don't get optimized away)
//
#pragma GCC push_options
#pragma GCC optimize ("O0")

#include <math.h>
#include <stdlib.h>
#include "sin_lut.h"
#include "hardwareAPI.h"

// testIterations is marked as "volatile" so that it can be updated with a debugger; otherwise
// it could be const or #define.
volatile uint32_t testIterations = 1000;

int main(int argc, char * argv[])
{
	errno_t err = 0;

	p_systemTime_t p_start = systemTime_create();
	p_systemTime_t p_end = systemTime_create();	
	double executionTime_scaffolding_ns = 0;
	double executionTime_scaffolding_ns_avg;
	double executionTime_sin_ns = 0;
	double executionTime_sin_ns_avg;
	double executionTime_sin_LUT_ns = 0;
	double executionTime_sin_LUT_ns_avg;
	double absoluteError_sin_LUT_sum = 0;
	double absoluteError_sin_LUT_avg;
	double percentError_sin_LUT_sum = 0;
	double percentError_sin_LUT_avg;

	// Check that p_start and p_end were allocated
	//
	ASSERT(p_start != NULL);
	ASSERT(p_end != NULL);
	
	// Initialize the underlying hardware (if necessary)
	//
	initHardware();

	// Seed the pseudo-random number generator
	//
	uint32_t time_ns;
	err = elapsedSystemTime_ns(&time_ns);
	ASSERT( err == 0 );
	srand((unsigned) time_ns);

	// Execute scaffolding with timing
	//
	for( int idx = 0; idx < testIterations; idx++ )
	{
		double output __attribute__((unused));
		double input = (double) rand() / (double) RAND_MAX * 2.0 * PI;
		err = getSystemTime(p_start);
		ASSERT( err == 0 );
		output = 0;
		err = getSystemTime(p_end);
		ASSERT( err == 0 );
		executionTime_scaffolding_ns += systemTimeDiff_ns(p_start, p_end);
	}
	executionTime_scaffolding_ns_avg = executionTime_scaffolding_ns / testIterations;

	// Execute library sin with timing
	//
	for( int idx = 0; idx < testIterations; idx++ )
	{
		double output __attribute__((unused));
		double input = (double) rand() / (double) RAND_MAX * 2.0 * PI;
		err = getSystemTime(p_start);
		ASSERT( err == 0 );
		output = sin( input );
		output = 0;
		err = getSystemTime(p_end);
		ASSERT( err == 0 );
		executionTime_sin_ns += systemTimeDiff_ns(p_start, p_end);
	}
	executionTime_sin_ns_avg = executionTime_sin_ns / testIterations;

	// Execute sin LUT with timing
	// 
	for( int idx = 0; idx < testIterations; idx++ )
	{
		double output __attribute__((unused));
		double input = (double) rand() / (double) RAND_MAX * 2.0 * PI;
		err = getSystemTime(p_start);
		ASSERT( err == 0 );
		output = sin_LUT( input );
		err = getSystemTime(p_end);
		ASSERT( err == 0 );
		executionTime_sin_LUT_ns += systemTimeDiff_ns(p_start, p_end);	
	}
	executionTime_sin_LUT_ns_avg = executionTime_sin_LUT_ns / testIterations;

	// Determine average percent error	
	//
	for( int idx = 0; idx < testIterations; idx++ )
	{
		double absoluteError;
		double percentError;

		// Compute input in radians
		//
		double rad = (double) rand() / (double) RAND_MAX * 2.0 * PI;

		// Compute outputs, for sin and sin_LUT
		//
		double output_sin = sin( rad );
		double output_sin_LUT = sin_LUT( rad );
		
		// Compute absolute error
		//
		absoluteError = fabs( output_sin - output_sin_LUT );
		absoluteError_sin_LUT_sum += absoluteError;

		// Compute percent error. Make sure the divisor, the expected value, is not 0
		//
		double expected = fabs(output_sin);
		if ( expected != 0 ) percentError = absoluteError / expected * 100.0;
		else percentError = 100.0;
		percentError_sin_LUT_sum += percentError;
	}
	absoluteError_sin_LUT_avg = absoluteError_sin_LUT_sum / testIterations;
	percentError_sin_LUT_avg = percentError_sin_LUT_sum / testIterations;
	
	printResults(testIterations, executionTime_scaffolding_ns, executionTime_scaffolding_ns_avg,
		executionTime_sin_ns_avg, executionTime_sin_LUT_ns_avg, 
		absoluteError_sin_LUT_avg, percentError_sin_LUT_avg);

	return EXIT_SUCCESS;
}

#pragma GCC pop_options