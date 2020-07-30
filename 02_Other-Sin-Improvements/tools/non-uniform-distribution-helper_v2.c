#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI (3.14159265358979000f)
#define TWO_PI (2.0*PI)
#define PI_OVER_TWO (PI/2.0)
#define THREE_PI_OVER_TWO (3.0*PI/2.0)

#define SQ(a) ((a)*(a))

int main(int argc, char * argv[])
{
	double max_error, error, x0, x1;
	int numElements;

	max_error = 0.00003;
	numElements = 0;

	// Hard code inflection points (points where function switches from increasing to decreasing or vice versa)
	// Artificially place an element in the LUT at each inflection point
	// Compute in a FORWARD direction (i.e. increasing x) the parts of the function which are DECREASING
	// Compute in a REVERSE direction (i.e. decreasing x) the parts of the function which are INCREASING
	// Reverse the results of the reverse direction so the x values appear in order

	// The inflection points for sin are at PI/2 and 3*PI/2

	// Start at PI/2 and work in REVERSE
	x1 = PI_OVER_TWO;
	printf("---X from PI/2 to 0---\n");
	while( x0 > 0 )
	{
		x0 = x1 - sqrt( ( max_error * 8 ) / ( fabs( sin( x1 ) ) ) );
		error = SQ( x1 - x0 ) * fabs( sin( x1 ) ) / 8;
		printf("%d:\tx: %1.20f\terror: %f\n", ++numElements, x1, error);
		x1 = x0;
	}

	// Now start again at PI/2 but, this time, work FORWARDS
	x0 = PI_OVER_TWO;
	printf("---X from PI/2 to PI---\n");
	while( x1 < PI )
	{
		x1 = sqrt( ( max_error * 8 ) / ( fabs( sin( x0 ) ) ) ) + x0;
		error = SQ( x1 - x0 ) * fabs( sin( x0 ) ) / 8;
		printf("%d:\tx: %1.20f\terror: %f\n", ++numElements, x0, error);
		x0 = x1;
	}

	// Next, start at 3*PI/2 and work in REVERSE
	x1 = THREE_PI_OVER_TWO;
	printf("---X from 3*PI/2 to PI---\n");
	while( x0 > PI )
	{
		x0 = x1 - sqrt( ( max_error * 8 ) / ( fabs( sin( x1 ) ) ) );
		error = SQ( x1 - x0 ) * fabs( sin( x1 ) ) / 8;
		printf("%d:\tx: %1.20f\terror: %f\n", ++numElements, x1, error);
		x1 = x0;
	}

	// Last, start again at 3*PI/2 but, this time, work FORWARDS
	x0 = THREE_PI_OVER_TWO;
	printf("---X from 3*PI/2 to 2*PI---\n");
	while( x1 < TWO_PI )
	{
		x1 = sqrt( ( max_error * 8 ) / ( fabs( sin( x0 ) ) ) ) + x0;
		error = SQ( x1 - x0 ) * fabs( sin( x0 ) ) / 8;
		printf("%d:\tx: %1.20f\terror: %f\n", ++numElements, x0, error);
		x0 = x1;
	}

}