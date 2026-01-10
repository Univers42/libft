cmake_minimum_required(VERSION 3.10)

project(libpipe C)

set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES
    "src/*.c"
)

# Create the library
add_library(pipe STATIC ${SOURCES})

# Example executable
add_executable(simple_pipeline examples/simple_pipeline.c)
target_link_libraries(simple_pipeline pipe)

# Tests
enable_testing()
add_subdirectory(tests)