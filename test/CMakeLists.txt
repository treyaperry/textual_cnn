
# Add source files
set(SOURCES
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
