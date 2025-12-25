#include <stdio.h>
#include <stdlib.h>

#include "tcnn/ErrorHandling.h"

static void print_usage() {
  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  fprintf(stderr, "Usage: tcnn <path_to_csv_file>\n");
  fprintf(stderr, "Example: tcnn data/textual_cnn/training/AI_Human.csv\n");
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
}

///
/// @brief Main entry point of the program.
/// @param argc Argument count.
/// @param argv Argument vector.
/// @return Exit status code.
///
int main(const int argc, const char **argv) {
  if (argc <= 1) {
    print_usage();
    return EXIT_FAILURE;
  }

  const char *csvPath = argv[1];
  FILE *csvFile = fopen(csvPath, "r");
  TCNN_ASSERT_OR_RETURN_VALUE(csvFile != NULL, EXIT_FAILURE,
                              "Failed to open CSV file");
  fclose(csvFile);

  return EXIT_SUCCESS;
}
