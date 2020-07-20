#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

#define MY_RAND_MAX UINT32_MAX

typedef int errno_t;
typedef struct systemTime_t * p_systemTime_t;

errno_t initHardware(void);

void seedPseudoRNG(uint32_t seed);
uint32_t getRand(void);

p_systemTime_t systemTime_create(void);
errno_t getSystemTime(p_systemTime_t p_thisTime);
uint32_t elapsedSystemTime_ns(void);
uint32_t systemTimeDiff_ns(p_systemTime_t p_startTime, p_systemTime_t p_endTime);

void printResults(uint32_t iterations, float executionTime_sin_ns_avg, 
	float executionTime_sin_LUT_ns_avg, float percentError_sin_LUT_avg);

#endif // HARDWARE_H
