/*
 * Utilities to help doing unit testing
 */

#ifndef _TEST_HELPER_H
#define _TEST_HELPER_H

#include <stdio.h>

unsigned int TESTS_PASSED = 0;
unsigned int TESTS_FAILED = 0;

#ifdef QUIET
const unsigned char VERBOSE = 0;
#else
const unsigned char VERBOSE = 1;
#endif

#define FAILED_TEST(...) do {\
    if (VERBOSE) \
        fprintf(stderr, __VA_ARGS__); \
    TESTS_FAILED++;\
} while (0)

#define PASSED_TEST do { \
    TESTS_PASSED++; \
} while (0)

#define EXPECT_EQUAL(_name_, _expected_, _actual_) do{\
    if ((_expected_) == (_actual_)) {\
        PASSED_TEST; \
    } else  { \
        FAILED_TEST("failed: %s: expected %d to be %d\n", (_name_), (_actual_), (_expected_));\
    }\
} while(0)

#define EXPECT_NOT_EQUAL(_name_, _expected_, _actual_) do{\
    if ((_expected_) != (_actual_)) {\
        PASSED_TEST; \
    } else { \
        FAILED_TEST("failed: %s: expected %s and %s to not be %d\n", (_name_), (#_expected_), (#_actual_), (_actual_)); \
    }\
} while(0)


#define SUMMARY do {\
    fprintf(stdout, "TESTS_PASSED: %d TESTS_FAILED: %d\n", TESTS_PASSED, TESTS_FAILED);\
} while (0)

#endif
