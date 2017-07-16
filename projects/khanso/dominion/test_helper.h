/*
 * Utilities to help doing unit testing
 */

#ifndef _TEST_HELPER_H
#define _TEST_HELPER_H

unsigned int TESTS_PASSED = 0;
unsigned int TESTS_FAILED = 0;

#define FAILED_TEST(_name_, _expected_, _actual_) do {\
    fprintf(stderr, "failed: %s: expected %d to be %d\n", (_name_), (_actual_), (_expected_));\
    TESTS_FAILED++;\
} while (0)


#define PASSED_TEST do { \
    TESTS_PASSED++; \
} while (0)

#define EXPECT_EQUAL(_name_, _expected_, _actual_) do{\
    if ((_expected_) == (_actual_)) {\
        PASSED_TEST; \
    } else  { \
        FAILED_TEST((_name_), (_expected_), (_actual_));\
    }\
} while(0)

#define SUMMARY do {\
    fprintf(stdout, "TESTS_PASSED: %d TESTS_FAILED: %d\n", TESTS_PASSED, TESTS_FAILED);\
} while (0)

#endif
