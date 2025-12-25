
# Add source files
set(SOURCES
    TextUtils.cpp
)

add_library(tcnn_core STATIC ${SOURCES})
target_include_directories(tcnn_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Create executable
add_executable(tcnn main.cpp)
target_link_libraries(tcnn tcnn_core)
