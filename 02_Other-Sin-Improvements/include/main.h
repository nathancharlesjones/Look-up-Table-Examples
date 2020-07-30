#ifndef MAIN_H
#define MAIN_H

#include "sin_lut.h"

typedef enum fcnSignature_t
{
	NOT_ASSIGNED,
	fcn_scaffolding,
	fcn_dbl_in_dbl_out,
	fcn_flt_in_flt_out,
	fcn_fxd_in_fxd_out
} fcnSignature_t;

typedef struct sinLUT_implementation_t
{
	const char * fcn_name;
	fcnSignature_t function_enum;
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
	double absoluteError_max;
	double percentError_sum;
	double percentError_avg;
} sinLUT_implementation_t;

#endif // MAIN_H