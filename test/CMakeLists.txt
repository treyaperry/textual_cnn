
# Add source files
set(SOURCES
    ErrorHandling_unittests.cpp
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

# Treat GoogleTest and GoogleMock includes as SYSTEM includes to suppress linter warnings associated with them.
target_include_directories(tcnn_unittests PRIVATE SYSTEM
    ${gtest_SOURCE_DIR}/include
    ${gmock_SOURCE_DIR}/include
)

# Register with CTest and enable shuffle + repeat
add_test(NAME tcnn_unittests
    COMMAND tcnn_unittests --gtest_shuffle --gtest_repeat=2
)
