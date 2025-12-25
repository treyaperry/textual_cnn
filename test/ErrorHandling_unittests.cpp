///
/// @par Tests for ErrorHandling functions.
///

#include "tcnn/ErrorHandling.hpp"

#include <gtest/gtest.h>

namespace {

///
/// @brief Tests the TCNN_ASSERT macro correctly triggers an assertion failure.
///
TEST(ErrorHandlingTest, Assert) {

  EXPECT_DEATH({ TCNN_ASSERT(false, "Assertion failure!"); }, "");
}

///
/// @brief Tests the TCNN_ASSERT_OR_RETURN macro correctly triggers an assertion
/// failure.
///
TEST(ErrorHandlingTest, AssertOrReturn) {

  EXPECT_DEATH({ TCNN_ASSERT_OR_RETURN(false, "Assertion failure!"); }, "");
}

///
/// @brief Tests the TCNN_ASSERT_OR_RETURN_VALUE macro correctly triggers an
/// assertion failure.
///
TEST(ErrorHandlingTest, AssertOrReturnValue) {

  /// A lambda is used here to provide a function context for the return value.
  auto func = []() -> int {
    TCNN_ASSERT_OR_RETURN_VALUE(false, -1, "Assertion failure!");
    return 0;
  };
  EXPECT_DEATH(func(), "");
}

///
/// @brief Tests the TCNN_ASSERT_OR_GOTO macro correctly triggers an assertion
/// failure.
///
TEST(ErrorHandlingTest, AssertOrGoto) {

  EXPECT_DEATH(
      {
        TCNN_ASSERT_OR_GOTO(false, failed_death, "Assertion failure!");
      failed_death:
        GTEST_FAIL() << "Should not reach here! for debug";
      },
      "");
}

///
/// @brief Tests the TCNN_ASSERT_FAIL macro correctly triggers an assertion
/// failure.
///
TEST(ErrorHandlingTest, AssertFail) {

  EXPECT_DEATH({ TCNN_ASSERT_FAIL("Fatal assertion failure!"); }, "");
}

} // namespace
