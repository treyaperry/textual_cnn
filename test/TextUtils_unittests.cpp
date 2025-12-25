///
/// @par Tests for TextUtils functions.
///

#include "tcnn/Macros.h"
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
/// @brief Tests TextUtils_to_grid_ids with empty string.
///
TEST(TextUtilsToGridIdsTest, EmptyString) {
  const TextGridParams PARAMS{.text = "", .width = 5, .maxRows = 2};
  image_id outIds[10];

  // Empty string should produce 1 row of PADs
  const size_t RESULT =
      ::TextUtils_to_grid_ids(&PARAMS, outIds, TCNN_ARRAY_LENGTH(outIds));
  EXPECT_EQ(5, RESULT); // 1 row * 5 width

  // All should be PAD_ID (0)
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[i]);
  }
}

///
/// @brief Tests TextUtils_to_grid_ids with text that fits in one row.
///
TEST(TextUtilsToGridIdsTest, SingleRowFitsExactly) {
  const TextGridParams PARAMS{.text = "Hi", .width = 5, .maxRows = 2};
  image_id outIds[10];

  const size_t RESULT =
      ::TextUtils_to_grid_ids(&PARAMS, outIds, TCNN_ARRAY_LENGTH(outIds));
  EXPECT_EQ(5, RESULT); // 1 row * 5 width

  // 'H' = 40, 'i' = 49, rest should be PAD (0)
  EXPECT_EQ(::TextUtils_char_to_id('H'), outIds[0]);
  EXPECT_EQ(::TextUtils_char_to_id('i'), outIds[1]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[2]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[3]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[4]);
}

///
/// @brief Tests TextUtils_to_grid_ids with text that spans multiple rows.
///
TEST(TextUtilsToGridIdsTest, MultipleRows) {
  const TextGridParams PARAMS{.text = "Hello World", .width = 5, .maxRows = 3};
  image_id outIds[15];

  const size_t RESULT =
      ::TextUtils_to_grid_ids(&PARAMS, outIds, TCNN_ARRAY_LENGTH(outIds));
  EXPECT_EQ(15, RESULT); // 3 rows * 5 width

  // Row 0: "Hello"
  EXPECT_EQ(::TextUtils_char_to_id('H'), outIds[0]);
  EXPECT_EQ(::TextUtils_char_to_id('e'), outIds[1]);
  EXPECT_EQ(::TextUtils_char_to_id('l'), outIds[2]);
  EXPECT_EQ(::TextUtils_char_to_id('l'), outIds[3]);
  EXPECT_EQ(::TextUtils_char_to_id('o'), outIds[4]);
  // Row 1: " Worl"
  EXPECT_EQ(::TextUtils_char_to_id(' '), outIds[5]);
  EXPECT_EQ(::TextUtils_char_to_id('W'), outIds[6]);
  EXPECT_EQ(::TextUtils_char_to_id('o'), outIds[7]);
  EXPECT_EQ(::TextUtils_char_to_id('r'), outIds[8]);
  EXPECT_EQ(::TextUtils_char_to_id('l'), outIds[9]);
  // Row 2: "d" + PADs
  EXPECT_EQ(::TextUtils_char_to_id('d'), outIds[10]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[11]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[12]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[13]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[14]);
}

///
/// @brief Tests TextUtils_to_grid_ids with text exceeding maxRows.
///
TEST(TextUtilsToGridIdsTest, TruncateAtMaxRows) {
  const TextGridParams PARAMS{.text = "ABCDEFGHIJ", .width = 3, .maxRows = 2};
  image_id outIds[10];

  const size_t RESULT =
      ::TextUtils_to_grid_ids(&PARAMS, outIds, TCNN_ARRAY_LENGTH(outIds));
  EXPECT_EQ(6, RESULT); // 2 rows * 3 width (truncates after 6 chars)

  // Row 0: "ABC"
  EXPECT_EQ(::TextUtils_char_to_id('A'), outIds[0]);
  EXPECT_EQ(::TextUtils_char_to_id('B'), outIds[1]);
  EXPECT_EQ(::TextUtils_char_to_id('C'), outIds[2]);

  // Row 1: "DEF"
  EXPECT_EQ(::TextUtils_char_to_id('D'), outIds[3]);
  EXPECT_EQ(::TextUtils_char_to_id('E'), outIds[4]);
  EXPECT_EQ(::TextUtils_char_to_id('F'), outIds[5]);
  // Characters 'G', 'H', 'I', 'J' are truncated
}

///
/// @brief Tests TextUtils_to_grid_ids with non-printable characters.
///
TEST(TextUtilsToGridIdsTest, NonPrintableChars) {
  const TextGridParams PARAMS{.text = "A\nB\tC", .width = 5, .maxRows = 2};
  image_id outIds[10];

  const size_t RESULT =
      ::TextUtils_to_grid_ids(&PARAMS, outIds, TCNN_ARRAY_LENGTH(outIds));
  EXPECT_EQ(5, RESULT); // 1 row * 5 width

  // 'A', PAD(\n), 'B', PAD(\t), 'C'
  EXPECT_EQ(::TextUtils_char_to_id('A'), outIds[0]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[1]); // '\n' -> PAD
  EXPECT_EQ(::TextUtils_char_to_id('B'), outIds[2]);
  EXPECT_EQ(TEXT_CONSTANTS_PAD_ID, outIds[3]); // '\t' -> PAD
  EXPECT_EQ(::TextUtils_char_to_id('C'), outIds[4]);
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

///
/// @brief Tests TextUtils_to_grid_ids crashes on zero width.
///
TEST(TextUtilsGridIdsDeathTest, ZeroWidth) {
  TextGridParams params{.text = "Hello", .width = 0, .maxRows = 2};
  image_id outIds[10];
  EXPECT_DEATH(::TextUtils_to_grid_ids(&params, outIds, 10), "");
}

///
/// @brief Tests TextUtils_to_grid_ids crashes on zero maxRows.
///
TEST(TextUtilsGridIdsDeathTest, ZeroMaxRows) {
  TextGridParams params{.text = "Hello", .width = 5, .maxRows = 0};
  image_id outIds[10];
  EXPECT_DEATH(::TextUtils_to_grid_ids(&params, outIds, 10), "");
}

} // namespace
