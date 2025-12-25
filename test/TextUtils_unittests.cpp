///
/// @par Tests for TextUtils functions.
///

#include "tcnn/TextUtils.hpp"

#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace {

using tcnn::ImageId;
using tcnn::TextGridParams;

struct PrintableCharParam {
  std::uint8_t characterCode;
  bool expectedIsPrintable;
};

struct IsPrintableCharTest : testing::TestWithParam<PrintableCharParam> {};

INSTANTIATE_TEST_SUITE_P(
    IsPrintableChar, IsPrintableCharTest,
    testing::Values(
        PrintableCharParam{.characterCode = 31, .expectedIsPrintable = false},
        PrintableCharParam{.characterCode = 32, .expectedIsPrintable = true},
        PrintableCharParam{.characterCode = 65, .expectedIsPrintable = true},
        PrintableCharParam{.characterCode = 126, .expectedIsPrintable = true},
        PrintableCharParam{.characterCode = 127,
                           .expectedIsPrintable = false}));

///
/// @brief Tests if an unsigned character code is printable.
///
TEST_P(IsPrintableCharTest, IsPrintableChar) {
  const auto &param{GetParam()};
  EXPECT_EQ(param.expectedIsPrintable,
            tcnn::is_printable_char(param.characterCode));
}

struct CharToIdParam {
  char character;
  tcnn::ImageId expectedId;
};

struct CharToIdTest : testing::TestWithParam<CharToIdParam> {};

INSTANTIATE_TEST_SUITE_P(
    CharToId, CharToIdTest,
    testing::Values(CharToIdParam{.character = ' ', .expectedId = ImageId{1}},
                    CharToIdParam{.character = 'A', .expectedId = ImageId{34}},
                    CharToIdParam{.character = '~', .expectedId = ImageId{95}},
                    CharToIdParam{
                        .character = '\n',
                        .expectedId = ImageId{0}}, // Non-printable character
                    CharToIdParam{.character = '\t', .expectedId = ImageId{0}}
                    // Non-printable character
                    ));

///
/// @brief Tests conversion from character to ID.
///
TEST_P(CharToIdTest, CharToId) {
  const auto &param{GetParam()};
  EXPECT_EQ(param.expectedId, tcnn::char_to_id(param.character));
}

///
/// @brief Placeholder test for TextUtils_to_grid_ids function.
/// @todo Implement tests once the function is implemented.
///
TEST(TextUtilsToGridIdsTest, PlaceholderTest) {
  constexpr TextGridParams params{
      .text = "sample text", .width = 10, .maxRows = 2};

  const auto ids{tcnn::to_grid_ids(params)};
  EXPECT_EQ(ids.size(), params.width * params.maxRows);
  EXPECT_TRUE(std::ranges::all_of(
      ids, [](const auto id) { return id == tcnn::text_pad_id; }));
}

} // namespace
