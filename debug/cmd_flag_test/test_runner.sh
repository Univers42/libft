#!/bin/bash
# Test script for cmd_flag_test functionality
# Tests the command-line flag parsing in ft_print_array

echo "=== CMD Flag Test Suite ==="
echo

# Build the test if it doesn't exist
if [ ! -f "./cmd_flag_test" ]; then
    echo "Building cmd_flag_test..."
    make TEST=cmd_flag_test test
    echo
fi

# Test 1: Default horizontal mode
echo "Test 1: Default horizontal mode"
echo "Command: ./cmd_flag_test 1 2 3 4 5"
echo "Expected: Numbers printed horizontally"
echo "Output:"
./cmd_flag_test 1 2 3 4 5
echo

# Test 2: Vertical mode with flag
echo "Test 2: Vertical mode with 'v' flag"
echo "Command: ./cmd_flag_test v 1 2 3 4 5"
echo "Expected: Numbers printed vertically with indices"
echo "Output:"
./cmd_flag_test v 1 2 3 4 5
echo

# Test 3: Horizontal mode with explicit flag
echo "Test 3: Horizontal mode with 'h' flag"
echo "Command: ./cmd_flag_test h 1 2 3 4 5"
echo "Expected: Numbers printed horizontally"
echo "Output:"
./cmd_flag_test h 1 2 3 4 5
echo

# Test 4: Negative numbers
echo "Test 4: Negative numbers with vertical flag"
echo "Command: ./cmd_flag_test v -5 0 10 -3"
echo "Expected: Negative numbers handled correctly"
echo "Output:"
./cmd_flag_test v -5 0 10 -3
echo

# Test 5: Single number
echo "Test 5: Single number"
echo "Command: ./cmd_flag_test 42"
echo "Expected: Single number printed"
echo "Output:"
./cmd_flag_test 42
echo

# Test 6: Empty arguments
echo "Test 6: Empty arguments"
echo "Command: ./cmd_flag_test"
echo "Expected: Exit code 1, empty output"
./cmd_flag_test
echo "Exit code: $?"
echo

echo "=== All tests completed ==="