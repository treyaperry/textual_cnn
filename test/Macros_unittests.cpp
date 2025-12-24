///
/// @par Tests for Macros functions.
///

#include "tcnn/Macros.h"

#include <gtest/gtest.h>

namespace {

using types = ::testing::Types<int, double, char, float>;

template <typename T> struct ArrayLengthTest : ::testing::Test {};

TYPED_TEST_SUITE(ArrayLengthTest, types);

///
/// @brief Tests the TCNN_ARRAY_LENGTH macro correctly computes the length of
/// statically-allocated arrays.
///
TYPED_TEST(ArrayLengthTest, ArraySize) {
  TypeParam arr[10];
  EXPECT_EQ(TCNN_ARRAY_LENGTH(arr), 10);
}

} // namespace
