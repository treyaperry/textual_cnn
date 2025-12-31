
# Add source files
set(SOURCES
    ErrorHandling_unittests.cpp
    Macros_unittests.cpp
    TextUtils_unittests.cpp  
)

# Create executable
add_executable(tcnn_unittests ${SOURCES})

# Link GoogleTest libraries
target_link_libraries(tcnn_unittests PRIVATE
    tcnn_core
    gtest 
    gtest_main
)

# Register with CTest and enable shuffle + repeat
add_test(NAME tcnn_unittests
    COMMAND tcnn_unittests --gtest_shuffle --gtest_repeat=2
)

target_compile_options(tcnn_unittests PRIVATE 
    # Gnu and Clang common warnings
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Wall # Enable most common warnings
        -Wextra # Additional warnings not covered by -Wall
        -Wpedantic # Strict ISO C++ conformance warnings
        -Werror # Treat all warnings as errors
        -Wshadow # Warn when a variable declaration shadows another variable
        -Wunused # Warn about unused variables, functions, etc.
        -Wconversion # Warn about implicit type conversions
        -Wsign-conversion # Warn about implicit sign conversions
        -Wformat=2 # Enhanced format string warnings
        -Wmissing-include-dirs # Warn about missing include directories
        -Wnon-virtual-dtor # Warn when class with virtual functions lacks a virtual destructor
        -Wold-style-cast # Warn about C-style casts
        -Woverloaded-virtual # Warn when function hides virtual functions
        -Wcast-align # Warn on casts that increase alignment requirements
    >
    $<$<CXX_COMPILER_ID:GNU>:
        -Wduplicated-cond # Warn about duplicated conditions in if-else-if chains
        -Wduplicated-branches # Warn about duplicated branches in if-else-if chains
        -Wlogical-op # Warn about suspicious uses of logical operators
        -Wuseless-cast # Warn about casts that do nothing
    >
    $<$<CXX_COMPILER_ID:Clang>:
        -Wnull-dereference # Warn about null pointer dereferences
        -Wdeprecated # Warn about deprecated declarations
        -Wdocumentation # Warn about non-conforming doxygen documentation
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4 # Enable most common warnings
        /WX # Treat all warnings as errors
        /permissive- # Enforce ISO C++ conformance
        /GS # Enable buffer security checks
        /guard:cf # Enable control flow guard. Mitigate security vulnerabilities.
        /RTCcsu # Enable runtime error checks for truncation checks, stack frame checks, uninitialized local variable usage checks.
    >
)
