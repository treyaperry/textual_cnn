///
/// @par Tests for TextUtils functions.
///

#include "tcnn/TextUtils.h"

#include <cstdint>
#include <gtest/gtest.h>

namespace {

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
            ::TextUtils_is_printable_char(param.characterCode));
}

struct CharToIdParam {
  char character;
  image_id expectedId;
};

struct CharToIdTest : testing::TestWithParam<CharToIdParam> {};

INSTANTIATE_TEST_SUITE_P(
    CharToId, CharToIdTest,
    testing::Values(CharToIdParam{.character = ' ', .expectedId = 1},
                    CharToIdParam{.character = 'A', .expectedId = 34},
                    CharToIdParam{.character = '~', .expectedId = 95},
                    CharToIdParam{.character = '\n',
                                  .expectedId = 0}, // Non-printable character
                    CharToIdParam{.character = '\t', .expectedId = 0}
                    // Non-printable character
                    ));

///
/// @brief Tests conversion from character to ID.
///
TEST_P(CharToIdTest, CharToId) {
  const auto &param{GetParam()};
  EXPECT_EQ(param.expectedId, ::TextUtils_char_to_id(param.character));
}

///
/// @brief Placeholder test for TextUtils_to_grid_ids function.
/// @todo Implement tests once the function is implemented.
///
TEST(TextUtilsToGridIdsTest, PlaceholderTest) {
  const TextGridParams PARAMS{.text = "sample text", .width = 10, .maxRows = 2};
  image_id outIds[2];

  // placeholder
  EXPECT_EQ(0, ::TextUtils_to_grid_ids(&PARAMS, outIds, 2));
}

///
/// @brief Tests TextUtils_to_grid_ids crashes on null PARAMS.
TEST(TextUtilsToGridIdsDeathTest, NullParams) {
  image_id outIds[10];
  EXPECT_DEATH(::TextUtils_to_grid_ids(nullptr, outIds, 10), "");
}

///
/// @brief Tests TextUtils_to_grid_ids crashes on null PARAMS->text.
///
TEST(TextUtilsToGridIdsDeathTest, NullText) {
  TextGridParams params;
  params.text = nullptr;
  image_id outIds[10];
  EXPECT_DEATH(::TextUtils_to_grid_ids(&params, outIds, 10), "");
}

///
/// @brief Tests TextUtils_to_grid_ids crashes on null outIds.
///
TEST(TextUtilsToGridIdsDeathTest, NullOutIds) {
  TextGridParams params{
    .text = "Hello",
  };
  EXPECT_DEATH(::TextUtils_to_grid_ids(&params, nullptr, 10), "");
}

} // namespace
