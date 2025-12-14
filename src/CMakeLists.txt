
# Add source files
set(SOURCES
    TextUtils.c
)

add_library(tcnn_core STATIC ${SOURCES})
target_include_directories(tcnn_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Create executable
add_executable(tcnn main.c)
target_link_libraries(tcnn tcnn_core)
