// Needed for clock_gettime()
// Has to be at the begining of the file
#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "hardwareAPI.h"

struct systemTime_t
{
	struct timespec thisTime;
};

errno_t initHardware(void)
{
	//No initialization required for x86
	return 0;
}

void seedPseudoRNG(uint32_t seed)
{
	srand((unsigned)seed);
}

uint32_t getRand(void)
{
	return rand();
}

p_systemTime_t systemTime_create(void)
{
	// TODO: Check for null pointer
	return (p_systemTime_t)calloc(1, sizeof(struct systemTime_t));
}

errno_t getSystemTime(p_systemTime_t p_thisTime)
{
	// TODO: Add null pointer checks
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &(p_thisTime->thisTime));
	return 0;
}

uint32_t elapsedSystemTime_ns(void)
{
	struct timespec now;

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
	return ( 1000000000 * now.tv_sec + now.tv_nsec );
}

uint32_t systemTimeDiff_ns(p_systemTime_t p_startTime, p_systemTime_t p_endTime)
{
	// TODO: Add null pointer checks
	return ( (1000000000 * (p_endTime->thisTime.tv_sec - p_startTime->thisTime.tv_sec)) + p_endTime->thisTime.tv_nsec - p_startTime->thisTime.tv_nsec );
}

void printResults(uint32_t iterations, float executionTime_sin_ns_avg, float executionTime_sin_LUT_ns_avg, float absoluteError_sin_LUT_avg, float percentError_sin_LUT_avg)
{
	printf("-----Sin LUT Test-----\n");
	printf("Number of iterations: %d\n", iterations);
	printf("Function\tAvg Execution Time (ns)\tAvg Absolute Error\tAvg Percent Error\n");
	printf("sin\t\t%f\t\tN/A\t\t\tN/A\n", executionTime_sin_ns_avg);
	printf("sin_LUT\t\t%f\t\t%f\t\t%f\n", executionTime_sin_LUT_ns_avg, absoluteError_sin_LUT_avg, percentError_sin_LUT_avg);
}