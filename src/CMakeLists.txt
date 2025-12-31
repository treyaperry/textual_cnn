
# Add source files
set(SOURCES
    TextUtils.cpp
)

add_library(tcnn_core STATIC ${SOURCES})
target_include_directories(tcnn_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(tcnn_core PUBLIC strong_type)

# Create executable
add_executable(tcnn main.cpp)
target_link_libraries(tcnn tcnn_core)

# compiler options
target_compile_options(tcnn_core PRIVATE 
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

target_compile_options(tcnn PRIVATE 
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
