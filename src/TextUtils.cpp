#include "tcnn/TextUtils.hpp"
#include "tcnn/ErrorHandling.hpp"
#include <vector>

namespace tcnn {

auto to_grid_ids(const TextGridParams &PARAMS,
                 std::vector<image_id> &outIds) noexcept -> void {
  TCNN_ASSERT_OR_RETURN(PARAMS.width > 0, "Width must be greater than zero.");
  TCNN_ASSERT_OR_RETURN(PARAMS.maxRows > 0,
                        "Max rows must be greater than zero.");

  // Preallocate maximum output size to avoid reallocations.
  const auto MAX_TEXT_LENGTH{PARAMS.width * PARAMS.maxRows};
  outIds.clear();
  outIds.reserve(MAX_TEXT_LENGTH);
}

} // namespace tcnn
