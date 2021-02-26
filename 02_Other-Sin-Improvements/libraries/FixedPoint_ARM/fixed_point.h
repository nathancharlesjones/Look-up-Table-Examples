#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>
#include "error.h"

//******************************************************//
//                                                      //
//  Adapted (heavily) from "Application Note 33: Fixed  //
//  Point Arithmetic on the ARM", Document Number: ARM  //
//  DAI 0033A, Issued: September 1996, Copyright        //
//  Advanced RISC Machines Ltd (ARM) 1996.              //
//                                                      //
//  https://developer.arm.com/documentation/dai0033/a/  //
//                                                      //
//******************************************************//

/* The basic operations perfomed on two numbers a and b of fixed
point q format returning the answer in q format */
#define FADD(a,b) ((a)+(b))
#define FSUB(a,b) ((a)-(b))
#define FMUL(a,b,q) (((a)*(b))>>(q))
#define FDIV(a,b,q) (((a)<<(q))/(b))

/* The basic operations where a is of fixed point q format and b is
an integer */
#define FADDI(a,b,q) ((a)+((b)<<(q)))
#define FSUBI(a,b,q) ((a)-((b)<<(q)))
#define FMULI(a,b) ((a)*(b))
#define FDIVI(a,b) ((a)/(b))

/* convert a from q1 format to q2 format */
#define FCONV(a, q1, q2) (((q2)>(q1)) ? (a)<<((q2)-(q1)) : (a)>>((q1)-(q2)))

/* the general operation between a in q1 format and b in q2 format
returning the result in q3 format */
#define FADDG(a,b,q1,q2,q3) (FCONV(a,q1,q3)+FCONV(b,q2,q3))
#define FSUBG(a,b,q1,q2,q3) (FCONV(a,q1,q3)-FCONV(b,q2,q3))
#define FMULG(a,b,q1,q2,q3) FCONV((a)*(b), (q1)+(q2), q3)
#define FDIVG(a,b,q1,q2,q3) (FCONV(a, q1, (q2)+(q3))/(b))

/* convert to and from floating point */
#define TOFIX(d, q) ((int)( (d)*(double)(1<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)(1<<(q)) )

//******************************************************//
//                                                      //
//  The following code was added by Nathan Jones, July  //
//  2020.                                               //
//                                                      //
//******************************************************//

// Define a fixed-point type with 1 sign bit (implied), 0 integer bits, and 31 fractional bits
typedef int32_t q0_31_t;

// Define a fixed-point type with 1 sign bit (implied), 9 integer bits, and 22 fractional bits
typedef int32_t q9_22_t;

static inline errno_t SAFE_FADD( int32_t a, int32_t b, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FADD( (int64_t)a, (int64_t)b );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FSUB( int32_t a, int32_t b, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FSUB( (int64_t)a, (int64_t)b );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FMUL( int32_t a, int32_t b, int32_t q, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FMUL( (int64_t)a, (int64_t)b, q );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FDIV( int32_t a, int32_t b, int32_t q, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FDIV( (int64_t)a, (int64_t)b, q );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FADDI( int32_t a, int32_t b, int32_t q, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FADDI( (int64_t)a, (int64_t)b, q );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FSUBI( int32_t a, int32_t b, int32_t q, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FSUBI( (int64_t)a, (int64_t)b, q );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FMULI( int32_t a, int32_t b, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FMULI( (int64_t)a, (int64_t)b );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FDIVI( int32_t a, int32_t b, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FDIVI( (int64_t)a, (int64_t)b );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FADDG( int32_t a, int32_t b, int32_t q1, int32_t q2, int32_t q3, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FADDG( (int64_t)a, (int64_t)b, q1, q2, q3 );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FSUBG( int32_t a, int32_t b, int32_t q1, int32_t q2, int32_t q3, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FSUBG( (int64_t)a, (int64_t)b, q1, q2, q3 );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FMULG( int32_t a, int32_t b, int32_t q1, int32_t q2, int32_t q3, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FMULG( (int64_t)a, (int64_t)b, q1, q2, q3 );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

static inline errno_t SAFE_FDIVG( int32_t a, int32_t b, int32_t q1, int32_t q2, int32_t q3, int32_t * ret)
{
	errno_t err = 0;

	int64_t _ret = FDIVG( (int64_t)a, (int64_t)b, q1, q2, q3 );

	if( ( _ret <= INT32_MAX ) && ( _ret >= INT32_MIN ) ) *ret = (int32_t)( _ret );
	else err = -1;

	return err;
}

#endif // FIXED_POINT_H