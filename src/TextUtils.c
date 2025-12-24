#include "tcnn/TextUtils.h"
#include "tcnn/ErrorHandling.h"

#include <stddef.h>

size_t TextUtils_to_grid_ids(const TextGridParams *PARAMS, image_id *outIds,
                             size_t outIdsSize) {
  TCNN_ASSERT_OR_RETURN_VALUE(PARAMS != NULL, 0, "PARAMS is NULL");
  TCNN_ASSERT_OR_RETURN_VALUE(PARAMS->text != NULL, 0, "PARAMS->text is NULL");
  TCNN_ASSERT_OR_RETURN_VALUE(outIds != NULL, 0, "outIds is NULL");
  return 0;
}
