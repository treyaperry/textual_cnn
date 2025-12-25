///
/// @par Text / character utilities
///

#ifndef TCNN_TEXT_UTILS_H
#define TCNN_TEXT_UTILS_H

#include <cstddef>
#include <cstdint>
#include <string_view>

#include <strong_type/equality.hpp>
#include <strong_type/ordered.hpp>
#include <strong_type/strong_type.hpp>

namespace tcnn {

using image_id = strong::type<std::int8_t, struct image_id_tag, strong::ordered,
                              strong::equality>;

inline constexpr image_id TEXT_PAD_ID{0};
inline constexpr image_id TEXT_VOCAB_SIZE{96}; // Number of printable ASCII
                                               // characters (from space to ~)
inline constexpr image_id TEXT_FIRST_PRINTABLE_CHAR{32};
inline constexpr image_id TEXT_LAST_PRINTABLE_CHAR{126};
inline constexpr image_id TEXT_PRINTABLE_RANGE_OFFSET{-31};

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
constexpr auto is_printable_char(const std::uint8_t CODE) noexcept -> bool {
  return (image_id{CODE} >= TEXT_FIRST_PRINTABLE_CHAR &&
          image_id{CODE} <= TEXT_LAST_PRINTABLE_CHAR);
}

///
/// @brief Converts a character to its corresponding ID in the range [1,
/// TEXT_CONSTANTS_VOCAB_SIZE]. The ID 0 is reserved for the padding character.
///
/// @param c The character to convert.
/// @return The corresponding ID of the character.
///
constexpr auto char_to_id(const char CHARACTER) noexcept -> image_id {
  const auto CODE{static_cast<std::uint8_t>(CHARACTER)};
  if (is_printable_char(CODE)) {
    return image_id{image_id{CODE}.value_of() +
                    TEXT_PRINTABLE_RANGE_OFFSET.value_of()};
  }
  return TEXT_PAD_ID;
}

///
/// @brief Converts a string of text into a grid of character IDs.
/// @param params Parameters for the text grid conversion.
///
/// @todo Implement this function - place holder for now.
auto to_grid_ids(const TextGridParams &params) noexcept -> void;

} // namespace tcnn

#endif // TCNN_TEXT_UTILS_H
