#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI (3.14159265358979000f)
#define TWO_PI (2.0*PI)
#define PI_OVER_TWO (PI/2.0)
#define THREE_PI_OVER_TWO (3.0*PI/2.0)

#define SQ(a) ((a)*(a))

int quadrantOf(double x)
{
	double _x = x;
	int ret;

	while( _x >= TWO_PI ) _x -= TWO_PI;
	while( _x < 0 ) _x += TWO_PI;

	if( _x <= PI_OVER_TWO ) ret = 1;
	else if ( _x <= PI ) ret = 2;
	else if ( _x <= THREE_PI_OVER_TWO ) ret = 3;
	else ret = 4;

	return ret;
}

int main(int argc, char * argv[])
{
	double max_error, error, x0, x1;
	int numElements, x0_quadrant, x1_quadrant;

	fprintf(stderr, "ERROR: This program has a bug; do not use.\n");
	exit(EXIT_FAILURE);

	max_error = 0.001;
	x0 = 0;
	x1 = x0 + PI_OVER_TWO;
	numElements = 0;

	do
	{
		// Detemine which quadrant x0 and x1 in
		// Q1 and Q1: Max is x1; Use Netwon's method to approximate x1
		// Q1 and Q2/3: Max is at pi/2; Calculate x1 directly
		// Q2/3 and Q2/3: Max is x0; Calculate x1 directly
		// Q2/3 and Q4: Max is at 3pi/2; Calculate x1 directly
		// Q4 and Q4: Max is x1; Use Newton's method to approximate x1

		do
		{
			x0_quadrant = quadrantOf(x0);
			x1_quadrant = quadrantOf(x1);

			if( ( ( x0_quadrant == 1 ) || ( x0_quadrant == 4 ) ) && ( x1_quadrant == 1 ) )
			{
				double x_prev = x1;
				double newtonsMethodFcn = ( SQ( x_prev - x0 ) * sin( x_prev ) / 8 ) - ( max_error * 0.9 );
				double newtonsMethodFcnPrime = ( x_prev * sin( x_prev ) / 4 ) + ( SQ( x_prev ) * cos(x_prev) / 8 ) + ( -2 * x0 * sin( x_prev ) / 8 ) + ( -2 * x0 * x_prev * cos( x_prev ) / 8 ) + ( SQ( x0 ) * cos( x_prev ) );
				x1 = x_prev - ( newtonsMethodFcn / newtonsMethodFcnPrime );
				error = SQ( x1 - x0 ) * sin( x1 ) / 8;
			}
			else if( ( x0_quadrant == 1 ) && ( ( x1_quadrant == 2 ) || ( x1_quadrant == 3 ) ) )
			{
				x1 = sqrt( ( max_error * 0.9 ) * 8 ) + x0;
				error = SQ( x1 - x0 ) / 8;
			}
			else if( ( ( x0_quadrant == 2 ) || ( x0_quadrant == 3 ) ) && ( ( x1_quadrant == 2 ) || ( x1_quadrant == 3 ) ) )
			{
				x1 = sqrt( ( max_error * 0.9 ) * 8 / fabs( sin(x0) ) ) + x0;
				error = SQ( x1 - x0 ) * fabs ( sin(x0) ) / 8;
			}
			else if( ( ( x0_quadrant == 2 ) || ( x0_quadrant == 3 ) ) && ( x1_quadrant == 4 ) )
			{
				x1 = sqrt( ( max_error * 0.9 ) * 8 ) + x0;
				error = SQ( x1 - x0 ) / 8;
			}
			else if( ( x0_quadrant == 4 ) && ( x1_quadrant == 4 ) )
			{
				double x_prev = x1;
				double newtonsMethodFcn = ( SQ( x_prev - x0 ) * sin( x_prev ) / 8 ) - ( max_error * 0.9 );
				double newtonsMethodFcnPrime = ( x_prev * sin( x_prev ) / 4 ) + ( SQ( x_prev ) * cos(x_prev) / 8 ) + ( -2 * x0 * sin( x_prev ) / 8 ) + ( -2 * x0 * x_prev * cos( x_prev ) / 8 ) + ( SQ( x0 ) * cos( x_prev ) );
				x1 = x_prev - ( newtonsMethodFcn / newtonsMethodFcnPrime );
				error = SQ( x1 - x0 ) * sin( x1 ) / 8;
			}
			else
			{
				fprintf(stderr, "x0 (quadrant %d) and x1 (quadrant %d) not in consecutive qudarants.\n", quadrantOf(x0), quadrantOf(x1));
				exit(EXIT_FAILURE);
			}
		} while( ( fabs( error ) > ( max_error ) ) || ( x0_quadrant != quadrantOf(x0) ) || ( x1_quadrant != quadrantOf(x1) ) );

		printf("%d:\tx: %1.10f\terror: %1.10f\n", ++numElements, x1, error);
		x0 = x1;
		x1 = x0 + PI_OVER_TWO;
	} while( x0 < TWO_PI );
}