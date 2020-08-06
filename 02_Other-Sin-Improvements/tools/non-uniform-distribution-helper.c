#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#include "assert.h"
#include "error.h"

#define PI (3.14159265358979000f)
#define TWO_PI (2.0*PI)
#define PI_OVER_TWO (PI/2.0)
#define THREE_PI_OVER_TWO (3.0*PI/2.0)

#define SQ(a) ((a)*(a))

int main(int argc, char * argv[])
{
	errno_t err;
	double max_error, x0, x1;
	int numElements;

	max_error = 0.007;
	numElements = 0;

	// Hard code inflection points (points where |f''(x)| switches from increasing to decreasing or vice versa)
	// Artificially place an element in the LUT at each inflection point
	// Compute in a FORWARD direction (i.e. increasing x) the parts of |f''(x)| which are DECREASING
	// Compute in a REVERSE direction (i.e. decreasing x) the parts of |f''(x)| which are INCREASING
	// Reverse the results of the reverse direction so the x values appear in order

	// The inflection points for sin are at PI/2 and 3*PI/2

	// Start at PI/2 and work in REVERSE
	p_node_t piOverTwoToZero = createNode(PI_OVER_TWO);
	ASSERT(piOverTwoToZero != NULL);
	numElements++;

	x1 = PI_OVER_TWO;
	while( ( x0 = x1 - sqrt( ( max_error * 8 ) / ( fabs( sin( x1 ) ) ) ) ) > 0 )
	{
		err = addNode( piOverTwoToZero, x0 );
		ASSERT(err == 0);
		numElements++;
		x1 = x0;
	}
	err = addNode( piOverTwoToZero, 0 );
	ASSERT(err == 0);
	numElements++;

	int idx;
	err = countItems(piOverTwoToZero, &idx);
	ASSERT(err == 0);
	for( idx; idx > 0; idx-- )
	{
		double item;
		err = getItem(piOverTwoToZero, idx-1, &item);
		ASSERT(err == 0);

		printf("{ %1.20f, %1.20f },\n", item, sin(item));
	}

	// Now start again at PI/2 but, this time, work FORWARDS
	x0 = PI_OVER_TWO;
	while( ( x1 = sqrt( ( max_error * 8 ) / ( fabs( sin( x0 ) ) ) ) + x0 ) < PI )
	{
		printf("{ %1.20f, %1.20f },\n", x1, sin(x1));
		numElements++;
		x0 = x1;
	}

	// Next, start at 3*PI/2 and work in REVERSE
	p_node_t threePiOverTwoToPi = createNode(THREE_PI_OVER_TWO);
	ASSERT(threePiOverTwoToPi != NULL);
	numElements++;

	x1 = THREE_PI_OVER_TWO;
	while( ( x0 = x1 - sqrt( ( max_error * 8 ) / ( fabs( sin( x1 ) ) ) ) ) > PI )
	{
		err = addNode( threePiOverTwoToPi, x0 );
		ASSERT(err == 0);
		numElements++;
		x1 = x0;
	}
	err = addNode( threePiOverTwoToPi, PI );
	ASSERT(err == 0);
	numElements++;

	err = countItems(threePiOverTwoToPi, &idx);
	ASSERT(err == 0);
	for( idx; idx > 0; idx-- )
	{
		double item;
		err = getItem(threePiOverTwoToPi, idx-1, &item);
		ASSERT(err == 0);

		printf("{ %1.20f, %1.20f },\n", item, sin(item));
	}

	// Last, start again at 3*PI/2 but, this time, work FORWARDS
	x0 = THREE_PI_OVER_TWO;
	while( ( x1 = sqrt( ( max_error * 8 ) / ( fabs( sin( x0 ) ) ) ) + x0 ) < TWO_PI )
	{
		printf("{ %1.20f, %1.20f },\n", x1, sin(x1));
		numElements++;
		x0 = x1;
	}
	printf("{ %1.20f, %1.20f },\n", TWO_PI, sin(TWO_PI));
	numElements++;
	printf("----------------------------------------------------\nTotal number of elements: %d\n", numElements);
}