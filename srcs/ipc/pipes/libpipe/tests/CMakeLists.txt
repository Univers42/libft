cmake_minimum_required(VERSION 3.10)

# Set the project name
project(libpipe_tests)

# Include the main library
include_directories(../include)

# Add test sources
set(TEST_SOURCES
    test_write_all.c
    test_read_all.c
)

# Create the test executable
add_executable(libpipe_tests ${TEST_SOURCES})

# Link the main library to the test executable
target_link_libraries(libpipe_tests PRIVATE pipe)