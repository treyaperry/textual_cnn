#include "tcnn/TextUtils.hpp"
#include "tcnn/ErrorHandling.hpp"
#include <vector>

namespace tcnn {

auto to_grid_ids(const TextGridParams &params) noexcept
    -> std::vector<ImageId> {
  TCNN_ASSERT_OR_RETURN_VALUE(params.width > 0, std::vector<ImageId>{},
                              "Width must be greater than zero.");
  TCNN_ASSERT_OR_RETURN_VALUE(params.maxRows > 0, std::vector<ImageId>{},
                              "Max rows must be greater than zero.");

  // Preallocate the output vector with padding IDs.
  std::vector<ImageId> ids(params.width * params.maxRows, text_pad_id);
  return ids;
}

} // namespace tcnn
