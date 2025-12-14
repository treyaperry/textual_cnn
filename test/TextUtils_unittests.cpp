///
/// @par Tests for TextUtils functions.
///

#include "tcnn/TextUtils.h"

#include <cstdint>
#include <gtest/gtest.h>

namespace
{

struct Param{
    std::uint8_t characterCode;
    bool expectedIsPrintable;
};

struct IsPrintableCharTest : testing::TestWithParam<Param>
{
};

INSTANTIATE_TEST_SUITE_P(
    IsPrintableChar,
    IsPrintableCharTest,
    testing::Values(
        Param{.characterCode = 31, .expectedIsPrintable = false},
        Param{.characterCode = 32, .expectedIsPrintable = true},
        Param{.characterCode = 65, .expectedIsPrintable = true},
        Param{.characterCode = 126, .expectedIsPrintable = true},
        Param{.characterCode = 127, .expectedIsPrintable = false}
    )
);

///
/// @brief Tests if an unsigned character code is printable.
///
TEST_P(IsPrintableCharTest, IsPrintableChar)
{
    const auto& param{GetParam()};
    EXPECT_EQ(param.expectedIsPrintable, ::TextUtils_is_printable_char(param.characterCode));
}   

}
