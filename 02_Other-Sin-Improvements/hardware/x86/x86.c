// Needed for clock_gettime()
// Has to be at the begining of the file
#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "hardwareAPI.h"
#include "assert.h"
#include "error.h"

void assert_failed(const char * file, uint32_t line)
{
	fprintf(stderr, "ERROR: Assert failed in %s at line %d\n", file, line);
	exit(-1);
}

struct systemTime_t
{
	struct timespec thisTime;
};

errno_t initHardware(void)
{
	//No initialization required for x86
	return 0;
}

p_systemTime_t systemTime_create(void)
{
	p_systemTime_t this = (p_systemTime_t)calloc(1, sizeof(struct systemTime_t));
	ASSERT(this != NULL);
	
	return this;
}

errno_t getSystemTime(p_systemTime_t p_thisTime)
{
	errno_t err = 0;

	ASSERT(p_thisTime != NULL);

	err = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &(p_thisTime->thisTime));
	
	return err;
}

errno_t elapsedSystemTime_ns(uint32_t * time_ns)
{
	errno_t err = 0;
	struct timespec now;

	ASSERT( time_ns != NULL );

	err = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
	if (err == 0) *time_ns = 1000000000 * now.tv_sec + now.tv_nsec;

	return err;
}

uint32_t systemTimeDiff_ns(p_systemTime_t p_startTime, p_systemTime_t p_endTime)
{
	ASSERT(p_startTime != NULL);
	ASSERT(p_endTime != NULL);

	// NOTE: No checks are made for overflow, meaning the results of this function are undefined
	// if the difference in nanoseconds between p_startTime and p_endTime is greater than UINT32_MAX
	// (i.e. greater than 4,294,967,295, or 4.294967295 seconds).
	uint32_t diff_sec = p_endTime->thisTime.tv_sec - p_startTime->thisTime.tv_sec;
	uint32_t diff_ns = p_endTime->thisTime.tv_nsec - p_startTime->thisTime.tv_nsec;
	
	return ( ( 1000000000 * diff_sec ) + diff_ns );
}

void printResults_CUT(uint32_t iterations, sinLUT_implementation_t codeUnderTest[])
{
	printf("-----Sin LUT Test-----\n");
	printf("Number of iterations: %d\n\n", iterations);
	printf("Function\tAvg Exec Time (ns)\tMax Abs Err\tAvg Abs Err\tAvg Pcnt Err\n");
	printf("------------------------------------------------------------------------------------\n");

	int idx_CUT = 0;
	while( codeUnderTest[idx_CUT].function_enum != NOT_ASSIGNED )
	{
		printf("%s\t%f\t\t%1.12f\t%1.12f\t%1.12f\n", codeUnderTest[idx_CUT].fcn_name, 
			codeUnderTest[idx_CUT].executionTime_ns_avg, 
			codeUnderTest[idx_CUT].absoluteError_max,
			codeUnderTest[idx_CUT].absoluteError_avg,
			codeUnderTest[idx_CUT].percentError_avg);
		idx_CUT++;
	}
}