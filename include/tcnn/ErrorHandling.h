///
/// @par Generic error handling utilities.
///

#ifndef TCNN_ERROR_HANDLING_H
#define TCNN_ERROR_HANDLING_H

#include <assert.h>

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

#define TCNN_ASSERT(CONDITION, MESSAGE)                                        \
  do {                                                                         \
    if (!(CONDITION)) {                                                        \
      TCNN_DIAG("Assertion failed: %s\nMessage: %s\n", #CONDITION, (MESSAGE)); \
      assert(CONDITION);                                                       \
    }                                                                          \
  } while (0)

#define TCNN_ASSERT_OR_RETURN(CONDITION, MESSAGE)                              \
  do {                                                                         \
    if (!(CONDITION)) {                                                        \
      TCNN_DIAG("Assertion failed: %s\nMessage: %s\n", #CONDITION, (MESSAGE)); \
      assert(CONDITION);                                                       \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define TCNN_ASSERT_OR_RETURN_VALUE(CONDITION, RETURN_VALUE, MESSAGE)          \
  do {                                                                         \
    if (!(CONDITION)) {                                                        \
      TCNN_DIAG("Assertion failed: %s\nMessage: %s\n", #CONDITION, (MESSAGE)); \
      assert(CONDITION);                                                       \
      return (RETURN_VALUE);                                                   \
    }                                                                          \
  } while (0)

#define TCNN_ASSERT_OR_GOTO(CONDITION, LABEL, MESSAGE)                         \
  do {                                                                         \
    if (!(CONDITION)) {                                                        \
      TCNN_DIAG("Assertion failed: %s\nMessage: %s\n", #CONDITION, (MESSAGE)); \
      assert(CONDITION);                                                       \
      goto LABEL;                                                              \
    }                                                                          \
  } while (0)

#define TCNN_ASSERT_FAIL(MESSAGE)                                              \
  do {                                                                         \
    TCNN_DIAG("Fatal assertion failure: %s\n", (MESSAGE));                     \
    assert(0);                                                                 \
    abort();                                                                   \
  } while (0)

/// @}

#endif // TCNN_ERROR_HANDLING_H
