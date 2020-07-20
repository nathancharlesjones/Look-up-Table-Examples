#include <math.h>
#include <stdlib.h>
#include "sin_lut.h"
#include "hardwareAPI.h"

#define TEST_ITERATIONS 10000
#define PI 3.14159265358979000f

int main(int argc, char * argv[])
{
	// TODO: Check for null pointers
	p_systemTime_t p_start = systemTime_create();
	p_systemTime_t p_end = systemTime_create();
	double executionTime_sin_ns = 0;
	double executionTime_sin_ns_avg;
	double executionTime_sin_LUT_ns = 0;
	double executionTime_sin_LUT_ns_avg;
	double absoluteError_sin_LUT_sum = 0;
	double absoluteError_sin_LUT_avg;
	double percentError_sin_LUT_sum = 0;
	double percentError_sin_LUT_avg;

	initHardware();

	// Seed the pseudo-random number generator (PRNG)
	seedPseudoRNG((unsigned) elapsedSystemTime_ns());

	// Execute library sin with timing
	for( int idx = 0; idx < TEST_ITERATIONS; idx++ )
	{
		double input = (double) getRand() / (double) MY_RAND_MAX * 2.0 * PI;
		getSystemTime(p_start);
		double output = sin( input );
		getSystemTime(p_end);
		executionTime_sin_ns += systemTimeDiff_ns(p_start, p_end);
	}
	executionTime_sin_ns_avg = executionTime_sin_ns / TEST_ITERATIONS;

	// Execute sin LUT with timing
	for( int idx = 0; idx < TEST_ITERATIONS; idx++ )
	{
		double input = (double) getRand() / (double) MY_RAND_MAX * 360.0;
		getSystemTime(p_start);
		double output = sin_LUT( input );
		getSystemTime(p_end);
		executionTime_sin_LUT_ns += systemTimeDiff_ns(p_start, p_end);	
	}
	executionTime_sin_LUT_ns_avg = executionTime_sin_LUT_ns / TEST_ITERATIONS;

	// Determine average percent error	
	for( int idx = 0; idx < TEST_ITERATIONS; idx++ )
	{
		double percentError;

		// Compute inputs, in degrees and radians
		//
		double deg = (double) getRand() / (double) MY_RAND_MAX * 360.0;
		double rad = deg * PI / 180.0;

		// Compute outputs, for sin and sin_LUT
		//
		double output_sin = sin( rad );
		double output_sin_LUT = sin_LUT( deg );
		
		// Compute absolute error
		//
		double absoluteError = fabs( output_sin - output_sin_LUT );
		absoluteError_sin_LUT_sum += absoluteError;

		// Compute percent error. Make sure the divisor, the expected value, is not 0
		//
		double expected = fabs(output_sin);
		if ( expected != 0 ) percentError = absoluteError / expected * 100.0;
		else percentError = 100.0;
		percentError_sin_LUT_sum += percentError;
	}
	absoluteError_sin_LUT_avg = absoluteError_sin_LUT_sum / TEST_ITERATIONS;
	percentError_sin_LUT_avg = percentError_sin_LUT_sum / TEST_ITERATIONS;
	
	printResults(TEST_ITERATIONS, executionTime_sin_ns_avg, executionTime_sin_LUT_ns_avg, absoluteError_sin_LUT_avg, percentError_sin_LUT_avg);

	return EXIT_SUCCESS;
}