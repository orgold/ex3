#ifndef TESTSMACROS_H_
#define TESTSMACROS_H_

#include <stdio.h>

#define TEST_EQUALS(result, a, b) if ((result) && ((a) != (b))) { \
								result = false; \
							}

#define TEST_DIFFERENT(result, a, b) if ((result) && ((a) == (b))) { \
								result = false; \
							}

#define TEST_TRUE(result, bool) if ((result) && !(bool)) { \
								result = false; \
							}

#define TEST_FALSE(result, bool) if ((result) && (bool)) { \
								result = false; \
							}

#define RUN_TEST(name)  printf("Running "); \
						printf(#name);		\
						printf("... ");		\
						if (!name()) { \
							printf("[FAILED]\n");		\
							return -1; \
						}								\
						printf("[SUCCESS]\n");

#endif /* TESTSMACROS_H_ */
