///
/// @par Tests for TextUtils functions.
///

#include "tcnn/TextUtils.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace {

using tcnn::ImageId;
using tcnn::TextGridParams;

struct PrintableCharParam {
  std::uint8_t character_code;
  bool expected_is_printable;
};

struct IsPrintableCharTest : testing::TestWithParam<PrintableCharParam> {};

INSTANTIATE_TEST_SUITE_P(
    IsPrintableChar, IsPrintableCharTest,
    testing::Values(
        PrintableCharParam{.character_code = 31,
                           .expected_is_printable = false},
        PrintableCharParam{.character_code = 32, .expected_is_printable = true},
        PrintableCharParam{.character_code = 65, .expected_is_printable = true},
        PrintableCharParam{.character_code = 126,
                           .expected_is_printable = true},
        PrintableCharParam{.character_code = 127,
                           .expected_is_printable = false}));

///
/// @brief Tests if an unsigned character code is printable.
///
TEST_P(IsPrintableCharTest /*unused*/, IsPrintableChar /*unused*/) {
  const auto &param{GetParam()};
  EXPECT_EQ(param.expected_is_printable,
            tcnn::is_printable_char(param.character_code));
}

struct CharToIdParam {
  char character;
  tcnn::ImageId expected_id;
};

struct CharToIdTest : testing::TestWithParam<CharToIdParam> {};

INSTANTIATE_TEST_SUITE_P(
    CharToId, CharToIdTest,
    testing::Values(CharToIdParam{.character = ' ', .expected_id = ImageId{1}},
                    CharToIdParam{.character = 'A', .expected_id = ImageId{34}},
                    CharToIdParam{.character = '~', .expected_id = ImageId{95}},
                    CharToIdParam{
                        .character = '\n',
                        .expected_id = ImageId{0}}, // Non-printable character
                    CharToIdParam{.character = '\t', .expected_id = ImageId{0}}
                    // Non-printable character
                    ));

///
/// @brief Tests conversion from character to ID.
///
TEST_P(CharToIdTest /*unused*/, CharToId /*unused*/) {
  const auto &param{GetParam()};
  EXPECT_EQ(param.expected_id, tcnn::char_to_id(param.character));
}

///
/// @brief Placeholder test for TextUtils_to_grid_ids function.
/// @todo Implement tests once the function is implemented.
///
TEST(TextUtilsToGridIdsTest /*unused*/, PlaceholderTest /*unused*/) {
  constexpr TextGridParams params{
      .text = "sample text", .width = 10, .max_rows = 2};

  std::vector<image_id> outIds;

  // Currently, the function is a placeholder and does not return any value.
  // This test simply ensures that the function can be called without errors.
  EXPECT_NO_FATAL_FAILURE(tcnn::to_grid_ids(PARAMS, outIds));
  EXPECT_EQ(outIds.size(), 0); // Since the function is not implemented yet
  EXPECT_EQ(outIds.capacity(),
            PARAMS.width * PARAMS.maxRows); // Preallocated size.
}

} // namespace
