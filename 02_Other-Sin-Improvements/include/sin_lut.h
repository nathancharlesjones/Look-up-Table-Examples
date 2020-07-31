#ifndef SIN_LUT_H
#define SIN_LUT_H

#include "fixed_point.h"

#define PI 3.14159265358979000f
#define TWO_PI (2.0*PI)

typedef double (*p_sin_LUT_double)(double);
typedef float (*p_sin_LUT_float)(float);
typedef q0_31_t (*p_sin_LUT_fixedPoint)(q9_22_t);

// Populate sin LUTs
void init_sinLUT(void);

// Default sin LUT implementation
double sin_LUT_double(double radians);

// Make the default faster/smaller by changing the data types being operated on
float sin_LUT_float(float radians);
q0_31_t sin_LUT_fixedPoint_safe(q9_22_t radians);
q0_31_t sin_LUT_fixedPoint(q9_22_t radians);

// Make the default more accurate/slightly slower by adding linear interpolation and/or non-uniform distribution
double sin_LUT_double_interpolate(double radians);
double sin_LUT_double_nonUniform(double radians);
float sin_LUT_float_interpolate(float radians);
float sin_LUT_float_nonUniform(float radians);
q0_31_t sin_LUT_fixedPoint_interpolate_safe(q9_22_t radians);
q0_31_t sin_LUT_fixedPoint_interpolate(q9_22_t radians);
q0_31_t sin_LUT_fixedPoint_nonUniform(q9_22_t radians);
q0_31_t sin_LUT_fixedPoint_nonUniform_safe(q9_22_t radians);

#endif // SIN_LUT_H