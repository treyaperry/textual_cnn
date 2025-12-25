///
/// @par Text / character utilities
///

#ifndef TCNN_TEXT_UTILS_H
#define TCNN_TEXT_UTILS_H

#include <cstddef>
#include <cstdint>

using image_id = std::int8_t;

enum TextConstants : image_id {
  TEXT_CONSTANTS_PAD_ID = 0,
  TEXT_CONSTANTS_VOCAB_SIZE =
      96, // Number of printable ASCII characters (from space to ~)
  TEXT_CONSTANTS_FIRST_PRINTABLE_CHAR = 32,
  TEXT_CONSTANTS_LAST_PRINTABLE_CHAR = 126,
  TEXT_CONSTANTS_PRINTABLE_RANGE_OFFSET = -31,
};

struct TextGridParams {
  const char *text = nullptr;
  std::size_t width = 0;
  std::size_t maxRows = 0;
};
///
/// @brief Checks if a character code is a printable ASCII character.
/// @param CODE The character code to check.
/// @return true if the character is printable, false otherwise.
///
constexpr auto TextUtils_is_printable_char(const std::uint8_t CODE) noexcept
    -> bool {
  return (CODE >= TEXT_CONSTANTS_FIRST_PRINTABLE_CHAR &&
          CODE <= TEXT_CONSTANTS_LAST_PRINTABLE_CHAR);
}

///
/// @brief Converts a character to its corresponding ID in the range [1,
/// TEXT_CONSTANTS_VOCAB_SIZE]. The ID 0 is reserved for the padding character.
///
/// @param c The character to convert.
/// @return The corresponding ID of the character.
///
constexpr auto TextUtils_char_to_id(const char CHARACTER) noexcept -> image_id {
  const auto CODE{static_cast<std::uint8_t>(CHARACTER)};
  if (TextUtils_is_printable_char(CODE)) {
    return static_cast<image_id>(static_cast<image_id>(CODE) +
                                 TEXT_CONSTANTS_PRINTABLE_RANGE_OFFSET);
  }
  return TEXT_CONSTANTS_PAD_ID;
}

///
/// @brief Converts a string of text into a grid of character IDs.
/// @param params Parameters for the text grid conversion.
///
/// @todo Implement this function - place holder for now.
auto TextUtils_to_grid_ids(const TextGridParams &params) noexcept -> void;

#endif // TCNN_TEXT_UTILS_H
