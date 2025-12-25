///
/// @par Tests for Macros functions.
///

#include "tcnn/Macros.h"

#include <gtest/gtest.h>

namespace {

using types = ::testing::Types<int, double, char, float>;

template <typename T> struct ArrayLengthTest : ::testing::Test {};
template <typename T> struct MinMaxTest : ::testing::Test {};

TYPED_TEST_SUITE(ArrayLengthTest, types);
TYPED_TEST_SUITE(MinMaxTest, types);

///
/// @brief Tests the TCNN_ARRAY_LENGTH macro correctly computes the length of
/// statically-allocated arrays.
///
TYPED_TEST(ArrayLengthTest, ArraySize) {
  TypeParam arr[10];
  EXPECT_EQ(TCNN_ARRAY_LENGTH(arr), 10);
}

///
/// @brief Tests the TCNN_MIN and MAX macros.
///
TYPED_TEST(MinMaxTest, MinMaxValue) {
  constexpr auto VAL1{static_cast<TypeParam>(0)};
  constexpr auto VAL2{static_cast<TypeParam>(1)};
  EXPECT_EQ(TCNN_MIN(VAL1, VAL2), VAL1);
  EXPECT_EQ(TCNN_MAX(VAL1, VAL2), VAL2);
}

} // namespace
