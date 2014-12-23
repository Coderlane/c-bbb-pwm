

#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include <stdio.h>

#define STRINGIFY(val) #val
#define STR(val) STRINGIFY(val)

/**
 * @brief Check to see if a and be are equal, return -1 if they are not.
 *
 * @param a The first value.
 * @param b The second value.
 *
 * @return -1 on failure, else continue processing.
 */
#define expect_eq(a, b) {\
	if(a != b) {\
		fprintf(stderr, "ERROR: %s != %s at line: %d\n",\
				STR(a), STR(b), __LINE__);\
		exit(-1);\
	}\
}

/**
 * @brief Check to see if a and be are ot nequal, return -1 if they are.
 *
 * @param a The first value.
 * @param b The second value.
 *
 * @return -1 on failure, else continue processing.
 */
#define expect_neq(a, b) {\
	if(a == b) {\
		fprintf(stderr, "ERROR: %s == %s at line: %d\n",\
				STR(a), STR(b), __LINE__);\
		exit(-1);\
	}\
}

#endif /* TEST_MACROS_H */
