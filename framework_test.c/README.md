# Extensible Testing Framework for Debug Functions

This framework provides an extensible way to test debugging functions in the libft library.

## Features

- Lookup table based test registration system
- Categorized tests for better organization
- Generic test execution functions
- Extensible for new test cases and test categories
- User-friendly output with color coding

## How to Add New Tests

1. To add a new test, locate the `init_tests()` function in `debug_tester.c`
2. Add a new test registration using the `register_test()` function

Example:

```c
register_test(
    "Test Name",                    // Name of the test
    "Test description",             // Description
    TEST_CATEGORY,                  // Category (TEST_ASCII, TEST_HEX, etc)
    DATA_TYPE,                      // Type of data (TEXT, BINARY, etc)
    SIZE,                           // Size of test data
    setup_function,                 // Function to setup test data
    test_function,                  // Function to run the test
    EXPECT_TYPE,                    // How to verify (VISUAL, AUTOMATIC)
    "Expected output description"   // Description of expected output
);
```

3. If needed, add custom data setup functions

## How to Add New Categories

1. Add a new category to the `test_category_t` enum
2. Add the category name to the `category_names` array in `run_tests_by_category()`
3. Create test functions specific to that category if needed
4. Register tests for the new category using `register_test()`

## Running the Tests

```
make run
```

## Example Test Output

```
===================================
    DEBUGGING FUNCTIONS TESTER
===================================

===================================
    TESTING ft_print_ascii
===================================

--- Testing ASCII - Normal Text ---
Description: Tests printing ASCII representation of standard text
Expected: Hello, World!
Actual:    Hello, World!
Does the output match the expected? (y/n): y
[PASS] ASCII - Normal Text

...
```
