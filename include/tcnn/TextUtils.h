///
/// @par Text / character utilities
///

#ifndef TCNN_UTILS_TEXT_H
#define TCNN_UTILS_TEXT_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef int8_t image_id;

enum TextConstants : image_id {
  TEXT_CONSTANTS_PAD_ID = 0,
  TEXT_CONSTANTS_VOCAB_SIZE =
      96, // Number of printable ASCII characters (from space to ~)
  TEXT_CONSTANTS_FIRST_PRINTABLE_CHAR = 32,
  TEXT_CONSTANTS_LAST_PRINTABLE_CHAR = 126,
  TEXT_CONSTANTS_PRINTABLE_RANGE_OFFSET = -31,
};

///
/// @brief Checks if a character code is a printable ASCII character.
/// @param CODE The character code to check.
/// @return true if the character is printable, false otherwise.
///
inline bool TextUtils_is_printable_char(const uint8_t CODE) {
  return (bool)(CODE >= TEXT_CONSTANTS_FIRST_PRINTABLE_CHAR &&
          CODE <= TEXT_CONSTANTS_LAST_PRINTABLE_CHAR);
}

///
/// @brief Converts a character to its corresponding ID in the range [1,
/// TEXT_CONSTANTS_VOCAB_SIZE]. The ID 0 is reserved for the padding character.
///
/// @param c The character to convert.
/// @return The corresponding ID of the character.
///
inline image_id TextUtils_char_to_id(const char CHARACTER) {
  const uint8_t CODE = (uint8_t)CHARACTER;
  if (TextUtils_is_printable_char(CODE)) {
    return (image_id)((image_id)CODE + TEXT_CONSTANTS_PRINTABLE_RANGE_OFFSET);
  }
  return TEXT_CONSTANTS_PAD_ID; // Padding character ID
}

///
/// @brief Converts a string of text into a grid of character IDs.
/// @param text The input text string.
/// @param width The width of the grid (number of characters)
/// @param maxRows The maximum number of rows in the grid.
///
/// @todo Implement this function - place holder for now.
void TextUtils_to_grid_ids(const char* text, size_t width, size_t maxRows);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // TCNN_UTILS_TEXT_H