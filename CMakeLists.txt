cmake_minimum_required(VERSION 3.30)

project(textual_cnn
    DESCRIPTION "A convolutional neural network which converts text to an image to check for AI generation."
    LANGUAGES C CXX
    VERSION 0.1.0
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compiler commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Enable testing
include (FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)
enable_testing() # Enable testing for CTest integration.

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)
