#ifndef ASSERT_H
#define ASSERT_H

#include <stdlib.h>
#include <stdint.h>

static inline void assert_failed(const char * file, uint32_t line)
{
	fprintf(stderr, "ERROR: Assert failed in %s at line %d\n", file, line);
	exit(-1);
}

#define ASSERT(expr)								\
	do {											\
		if (!(expr)) {								\
			assert_failed(__FILENAME__, __LINE__);	\
		}											\
	} while (0)

#endif // ASSERT_H