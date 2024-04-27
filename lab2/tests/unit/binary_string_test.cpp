#include "binary_string.h"

#include <gtest/gtest.h>

TEST(binary_string_test, bitdifftest01) {
    const char *a = "a";
    const char *b = "b";
    EXPECT_EQ(BitDifference(a, b), 0);
}

TEST(binary_string_test, bitdifftest02) {
    const char *a = "wodlt";
    const char *b = "word";
    EXPECT_EQ(BitDifference(a, b), 17);
}
