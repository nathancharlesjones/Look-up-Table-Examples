#ifndef MAIN_H
#define MAIN_H

#include "sin_lut.h"

typedef enum sinLUT_function_t
{
	NOT_ASSIGNED,
	fcn_scaffolding,
	fcn_sin,
	fcn_sin_LUT_double,
	fcn_sin_LUT_float,
	fcn_sin_LUT_fixedPoint,
	fcn_sin_LUT_double_interpolate,
	fcn_sin_LUT_double_nonUniform,
	fcn_sin_LUT_double_rangeReduction,
	fcn_sin_LUT_float_interpolate
} sinLUT_function_t;

typedef struct sinLUT_implementation_t
{
	const char * fcn_name;
	sinLUT_function_t function_enum;
	union
	{
		p_sin_LUT_double fcn_double;
		p_sin_LUT_float fcn_float;
		p_sin_LUT_fixedPoint fcn_fixedPoint;
	};
	double executionTime_ns;
	double executionTime_ns_avg;
	double absoluteError_sum;
	double absoluteError_avg;
	double percentError_sum;
	double percentError_avg;
} sinLUT_implementation_t;

#endif // MAIN_H