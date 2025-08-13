# Command Flag Test for ft_print_array

This test demonstrates the command-line flag parsing functionality for the `ft_print_array` function.

## Overview

The test implements command-line argument parsing that supports different display modes for printing integer arrays:

- **Horizontal mode** (`h` or default): Prints numbers in a single line separated by spaces
- **Vertical mode** (`v`): Prints numbers vertically with array indices

## Building

To build the test executable:

```bash
make TEST=cmd_flag_test test
```

## Usage

```bash
./cmd_flag_test [mode] number1 [number2 ...]
```

### Parameters

- `mode` (optional): Display mode flag
  - `h`: Horizontal mode (default)
  - `v`: Vertical mode
- `number1, number2, ...`: Integer values to display

### Examples

1. **Default horizontal mode:**
   ```bash
   ./cmd_flag_test 1 2 3 4 5
   # Output: 1 2 3 4 5
   ```

2. **Vertical mode:**
   ```bash
   ./cmd_flag_test v 1 2 3 4 5
   # Output:
   # [0] --> 1 
   # [1] --> 2 
   # [2] --> 3 
   # [3] --> 4 
   # [4] --> 5 
   ```

3. **Explicit horizontal mode:**
   ```bash
   ./cmd_flag_test h 1 2 3 4 5
   # Output: 1 2 3 4 5
   ```

4. **With negative numbers:**
   ```bash
   ./cmd_flag_test v -5 0 10 -3
   # Output:
   # [0] --> -5 
   # [1] --> 0 
   # [2] --> 10 
   # [3] --> -3 
   ```

## Testing

Run the complete test suite:

```bash
./debug/cmd_flag_test/test_runner.sh
```

This will automatically build the test if needed and run all test cases to verify functionality.

## Implementation Details

The test demonstrates:
- Command-line argument parsing
- Flag detection and processing
- Differentiation between mode flags and numeric arguments
- Proper handling of edge cases (empty input, single numbers, negative values)
- Memory management with dynamic allocation
- Integration with existing libft functions

The functionality is based on the originally commented-out main function in `debug/ft_print_array.c`, now implemented as a standalone test.