#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

void assert_failed(const char * file, uint32_t line);

#define ASSERT(expr)								\
	do {											\
		if (!(expr)) {								\
			assert_failed(__FILENAME__, __LINE__);	\
		}											\
	} while (0)

typedef int 					errno_t;
typedef struct systemTime_t * 	p_systemTime_t;

errno_t initHardware(void);

p_systemTime_t systemTime_create(void);
errno_t getSystemTime(p_systemTime_t p_thisTime);
errno_t elapsedSystemTime_ns(uint32_t * time_ns);
uint32_t systemTimeDiff_ns(p_systemTime_t p_startTime, p_systemTime_t p_endTime);

void printResults(uint32_t iterations, float executionTime_scaffolding_ns, float executionTime_scaffolding_ns_avg,
	float executionTime_sin_ns_avg, float executionTime_sin_LUT_ns_avg, float absoluteError_sin_LUT_max,
	float absoluteError_sin_LUT_avg, float percentError_sin_LUT_avg);

#endif // HARDWARE_H
