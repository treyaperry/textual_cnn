#include "tcnn/TextUtils.h"
#include "tcnn/ErrorHandling.h"

#include <stddef.h>

constexpr size_t ZERO_WRITTEN_ELEMENTS = 0;

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
  return 0;
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
