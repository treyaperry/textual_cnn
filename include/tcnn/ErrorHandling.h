///
/// @par Generic error handling utilities.
///

#ifndef TCNN_ERROR_HANDLING_H
#define TCNN_ERROR_HANDLING_H

#include <assert.h>

#include "tcnn/Macros.h"

// Debug-only diagnostic: prints in debug, compiles to no-op in release.
#ifndef NDEBUG
#include <stdio.h> // IWYU pragma: keep
#define TCNN_DIAG(...) fprintf(stderr, __VA_ARGS__)
#else
#define TCNN_DIAG(...) ((void)0)
#endif // NDEBUG

///
/// @brief Assertion macros.
/// - All macros print a diagnostic to stderr.
/// - They call assert() so debuggers will break in debug builds.
/// - Variants that return/goto execute the control-flow action in both debug
///   and release builds (assert() is a no-op when NDEBUG is defined).
/// @{
///

#define TCNN_ASSERT(condition, message)                                        \
  do {                                                                         \
    if (!(condition)) {                                                        \
      TCNN_DIAG("Assertion failed: %s\nMessage: %s\n",                         \
                TCNN_STRINGIFY(condition), (message));                         \
      assert(condition);                                                       \
    }                                                                          \
  } while (0)

#define TCNN_ASSERT_OR_RETURN(condition, message)                              \
  do {                                                                         \
    if (!(condition)) {                                                        \
      TCNN_DIAG("Assertion failed: %s\nMessage: %s\n",                         \
                TCNN_STRINGIFY(condition), (message));                         \
      assert(condition);                                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define TCNN_ASSERT_OR_RETURN_VALUE(condition, RETURN_VALUE, message)          \
  do {                                                                         \
    if (!(condition)) {                                                        \
      TCNN_DIAG("Assertion failed: %s\nMessage: %s\n",                         \
                TCNN_STRINGIFY(condition), (message));                         \
      assert(condition);                                                       \
      return (RETURN_VALUE);                                                   \
    }                                                                          \
  } while (0)

#define TCNN_ASSERT_OR_GOTO(condition, LABEL, message)                         \
  do {                                                                         \
    if (!(condition)) {                                                        \
      TCNN_DIAG("Assertion failed: %s\nMessage: %s\n",                         \
                TCNN_STRINGIFY(condition), (message));                         \
      assert(condition);                                                       \
      goto LABEL;                                                              \
    }                                                                          \
  } while (0)

#define TCNN_ASSERT_FAIL(message)                                              \
  do {                                                                         \
    TCNN_DIAG("Fatal assertion failure: %s\n", (message));                     \
    assert(0);                                                                 \
    abort();                                                                   \
  } while (0)

/// @}

#endif // TCNN_ERROR_HANDLING_H
