#include "tcnn/TextUtils.h"
#include "tcnn/ErrorHandling.h"
#include "tcnn/Macros.h"

#include <stddef.h>
#include <string.h>

constexpr size_t ZERO_WRITTEN_ELEMENTS = 0;

///
/// @brief Fills a range in the output array with PAD_IDs.
/// @param START_INDEX Starting index of the range (inclusive).
/// @param END_INDEX Ending index of the range (exclusive).
/// @param outIds Output array to hold the character IDs.
///
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
static void fill_with_pad_ids(const size_t START_INDEX, const size_t END_INDEX,
                              image_id *outIds) {
  if (START_INDEX > END_INDEX) {
    return;
  }

  // The below buffer handling is safe because the length is controlled.
  // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  memset(&outIds[START_INDEX], TEXT_CONSTANTS_PAD_ID, END_INDEX - START_INDEX);
}

///
/// @brief Pads the output with a PAD_ID if the text is empty.
/// @param TEXT_LENGTH Length of the input
/// @param outIds Output array to hold the character IDS.
///
/// @return True if padding was added, false otherwise.
///
static bool insert_pad_if_empty(const size_t TEXT_LENGTH, image_id *outIds) {
  if (TEXT_LENGTH > 0) {
    return false;
  }
  fill_with_pad_ids(0, 1, outIds);
  return true;
}

///
/// @brief Converts text to character IDs.
/// @param TEXT Input text to convert.
/// @param MAX_TEXT_LENGTH Maximum length of text to process.
/// @param outIds Output array to hold the character IDs.
///
/// @return The number of character IDs written to the output array.
///
static size_t convert_to_ids(const char *const TEXT,
                             const size_t MAX_TEXT_LENGTH, image_id *outIds) {
  size_t textLength = 0;
  const char *currentChar = TEXT;
  while ((*currentChar != '\0') && (textLength < MAX_TEXT_LENGTH)) {
    outIds[textLength] = TextUtils_char_to_id(*currentChar);
    ++textLength;
    ++currentChar;
  }

  if (insert_pad_if_empty(textLength, outIds)) {
    textLength = 1;
  }
  return textLength;
}

///
/// @brief Computes the required number of rows for the text grid.
/// @param PARAMS Parameters for the text grid conversion.
/// @param TEXT_LENGTH Length of the input text.
///
/// @return The required number of rows.
///
static size_t compute_required_rows(const TextGridParams *const PARAMS,
                                    const size_t TEXT_LENGTH) {
  const size_t ROWS = (TEXT_LENGTH + PARAMS->width - 1) / PARAMS->width;
  return TCNN_MIN(ROWS, PARAMS->maxRows);
}

///
/// @brief Computes the grid size based on parameters and text length.
/// @param PARAMS Parameters for the text grid conversion.
/// @param TEXT_LENGTH Length of the input text.
///
/// @return The grid capacity.
///
static size_t compute_grid_size(const TextGridParams *const PARAMS,
                                const size_t TEXT_LENGTH) {
  const size_t REQUIRED_ROWS = compute_required_rows(PARAMS, TEXT_LENGTH);
  return REQUIRED_ROWS * PARAMS->width;
}

///
/// @brief To grid ids implementation.
///
/// This function is called when all input validation checks have passed.
///
/// @param PARAMS Parameters for the text grid conversion.
/// @param MAX_TEXT_LENGTH Pre-calculated maximum length of text to process.
/// @param outIds Output array to hold the character IDs.
///
/// @return The number of character IDs written to the output array.
///
static size_t to_grid_ids_impl(const TextGridParams *PARAMS,
                               const size_t MAX_TEXT_LENGTH, image_id *outIds) {
  const size_t IDS_CONVERTED_COUNT =
      convert_to_ids(PARAMS->text, MAX_TEXT_LENGTH, outIds);

  const size_t GRID_SIZE = compute_grid_size(PARAMS, IDS_CONVERTED_COUNT);

  fill_with_pad_ids(IDS_CONVERTED_COUNT, GRID_SIZE, outIds);

  return GRID_SIZE;
}

size_t TextUtils_to_grid_ids(const TextGridParams *PARAMS, image_id *outIds,
                             const size_t OUT_IDS_CAPACITY) {
  TCNN_ASSERT_OR_RETURN_VALUE(PARAMS != NULL, ZERO_WRITTEN_ELEMENTS,
                              "PARAMS is NULL");
  TCNN_ASSERT_OR_RETURN_VALUE(PARAMS->text != NULL, ZERO_WRITTEN_ELEMENTS,
                              "PARAMS->text is NULL");
  TCNN_ASSERT_OR_RETURN_VALUE(outIds != NULL, ZERO_WRITTEN_ELEMENTS,
                              "outIds is NULL");
  TCNN_ASSERT_OR_RETURN_VALUE(PARAMS->width > 0, ZERO_WRITTEN_ELEMENTS,
                              "PARAMS->width is 0");
  TCNN_ASSERT_OR_RETURN_VALUE(PARAMS->maxRows > 0, ZERO_WRITTEN_ELEMENTS,
                              "PARAMS->maxRows is 0");

  const size_t MAX_TEXT_LENGTH = PARAMS->width * PARAMS->maxRows;
  TCNN_ASSERT_OR_RETURN_VALUE(OUT_IDS_CAPACITY >= MAX_TEXT_LENGTH,
                              ZERO_WRITTEN_ELEMENTS,
                              "OUT_IDS_CAPACITY is too small");

  return to_grid_ids_impl(PARAMS, MAX_TEXT_LENGTH, outIds);
}
