///
/// @par Tests for Macros functions.
///

#include "tcnn/Macros.h"

#include <gtest/gtest.h>

namespace {

///
/// @brief Tests the TCNN_STRINGIFY macro correctly converts tokens to
/// strings.
///
TEST(MacrosTest, Stringify) {
  EXPECT_STREQ(TCNN_STRINGIFY(HelloWorld), "HelloWorld");
}

} // namespace
