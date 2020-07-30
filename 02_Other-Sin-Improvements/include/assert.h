#ifndef ASSERT_H
#define ASSERT_H

void assert_failed(const char * file, uint32_t line);

#define ASSERT(expr)								\
	do {											\
		if (!(expr)) {								\
			assert_failed(__FILENAME__, __LINE__);	\
		}											\
	} while (0)

#endif // ASSERT_H