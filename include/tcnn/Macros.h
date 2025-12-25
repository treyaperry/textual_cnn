///
/// @par General-purpose utility macros.
///

#ifndef TCNN_MACROS_H
#define TCNN_MACROS_H

///
/// @brief Get the number of elements in a statically-allocated array.
///
#define TCNN_ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

///
/// @brief Stringify a macro argument.
///
#define TCNN_INTERNAL_STRINGIFY(x) #x
#define TCNN_STRINGIFY(x) TCNN_INTERNAL_STRINGIFY(x)

#define TCNN_MIN(a, b) ((a) < (b) ? (a) : (b))
#define TCNN_MAX(a, b) ((a) > (b) ? (a) : (b))

#endif // TCNN_MACROS_H
