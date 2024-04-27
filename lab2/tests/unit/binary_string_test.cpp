#include "binary_string.h"

#include <gtest/gtest.h>

TEST(binary_string_test, bitdifftest01) {
    const char *a = "a";  
    const char *b = "b";  
    EXPECT_EQ(BitDifference(a, b), 0);
}
