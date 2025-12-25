#include "tcnn/TextUtils.h"
#include "tcnn/ErrorHandling.h"

#include <stddef.h>

enum WrittenElements : size_t {
  ZERO_WRITTEN_ELEMENTS = 0,
};

///
/// @brief To grid ids implementation.
///
/// This function is called when all input validation checks have passed.
///
/// @param PARAMS Parameters for the text grid conversion.
/// @param outIds Output array to hold the character IDs.
/// @param outIdsCapacity Capacity of the output array.
///
/// @return The number of character IDs written to the output array.
///
static size_t TextUtils_to_grid_ids_impl(const TextGridParams *PARAMS,
                                         image_id *outIds,
                                         size_t outIdsCapacity) {
  return 0;
}

size_t TextUtils_to_grid_ids(const TextGridParams *PARAMS, image_id *outIds,
                             size_t outIdsCapacity) {
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
  TCNN_ASSERT_OR_RETURN_VALUE(
      outIdsCapacity >= (PARAMS->width * PARAMS->maxRows),
      ZERO_WRITTEN_ELEMENTS, "outIdsCapacity is too small");

  return TextUtils_to_grid_ids_impl(PARAMS, outIds, outIdsCapacity);
}
