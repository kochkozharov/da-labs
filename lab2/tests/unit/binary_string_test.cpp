#include "binary_string.h"

#include <gtest/gtest.h>

TEST(binary_string_test, bitdifftest01) {
    const char *a = "a";  // 00001
    const char *b = "b";  // 00001
    EXPECT_EQ(BitDifference(a, b), 3);
}
