#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>

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

// Define a fixed-point type with 1 sign bit (implied), 15 integer bits, and 16 fractional bits
typedef int32_t q15_16_t;

// Define a fixed-point type with 1 sign bit (implied), 25 integer bits, and 6 fractional bits
typedef int32_t q25_6_t;

#endif // FIXED_POINT_H