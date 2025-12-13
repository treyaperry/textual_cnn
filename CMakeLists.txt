cmake_minimum_required(VERSION 3.30)

project(textual_cnn
    DESCRIPTION "A convolutional neural network which converts text to an image to check for AI generation."
    LANGUAGES C
    VERSION 0.1.0
)

set(CMAKE_C_STANDARD 23)

# Export compiler commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Add subdirectories
add_subdirectory(src)
