///
/// @par Text / character utilities
///

#ifndef TCNN_TEXT_UTILS_HPP
#define TCNN_TEXT_UTILS_HPP

#include <cstddef>
#include <cstdint>
#include <string_view>

#include <strong_type/equality.hpp>
#include <strong_type/ordered.hpp>
#include <strong_type/strong_type.hpp>

namespace tcnn {

using ImageId = strong::type<std::int8_t, struct ImageIdTag, strong::ordered,
                             strong::equality>;

inline constexpr ImageId text_pad_id{0};
inline constexpr ImageId text_vocab_size{96}; // Number of printable ASCII
                                              // characters (from space to ~)
inline constexpr ImageId text_first_printable_char{32};
inline constexpr ImageId text_last_printable_char{126};
inline constexpr ImageId text_printable_range_offset{-31};

struct TextGridParams {
  std::string_view text;
  std::size_t width{0};
  std::size_t maxRows{0};
};

///
/// @brief Checks if a character code is a printable ASCII character.
/// @param CODE The character code to check.
/// @return true if the character is printable, false otherwise.
///
[[nodiscard]] constexpr auto is_printable_char(const std::uint8_t CODE) noexcept
    -> bool {
  return (ImageId{CODE} >= text_first_printable_char &&
          ImageId{CODE} <= text_last_printable_char);
}

///
/// @brief Converts a character to its corresponding ID in the range [1,
/// TEXT_CONSTANTS_VOCAB_SIZE]. The ID 0 is reserved for the padding character.
///
/// @param c The character to convert.
/// @return The corresponding ID of the character.
///
[[nodiscard]] constexpr auto char_to_id(const char CHARACTER) noexcept
    -> ImageId {
  const auto CODE{static_cast<std::uint8_t>(CHARACTER)};
  if (is_printable_char(CODE)) {
    return ImageId{ImageId{CODE}.value_of() +
                   text_printable_range_offset.value_of()};
  }
  return text_pad_id;
}

///
/// @brief Converts a string of text into a grid of character IDs.
/// @param params Parameters for the text grid conversion.
/// @param outIds Output vector to store the resulting character IDs.
/// @return A vector of character IDs representing the text grid.
///
/// @todo Implement this function - place holder for now.
auto to_grid_ids(const TextGridParams &params) noexcept -> std::vector<ImageId>;

} // namespace tcnn

#endif // TCNN_TEXT_UTILS_HPP
