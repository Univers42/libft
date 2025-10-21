# Enhanced General Purpose Test Framework - Quick Reference

## Overview
A comprehensive C testing framework that supports:
- ✅ Function comparison (1-4 parameters)
- ✅ Variable comparison (any type)
- ✅ Edge case validation
- ✅ Custom comparators
- ✅ Batch testing
- ✅ Detailed reporting

---

## Core Concepts

### 1. Test Suite
Container for all your tests:
```c
t_test_suite *suite = TEST_SUITE("My test suite");
// ... run tests ...
test_suite_print_summary(suite);
test_suite_destroy(suite);
```

### 2. Comparison Types
```c
CMP_INT       // Integer comparison
CMP_UINT      // Unsigned integer
CMP_CHAR      // Character
CMP_FLOAT     // Float (with epsilon)
CMP_DOUBLE    // Double (with epsilon)
CMP_PTR       // Pointer comparison
CMP_STRING    // String comparison (strcmp)
CMP_MEMORY    // Memory block (memcmp)
CMP_BOOL      // Truthiness (!!value)
CMP_CUSTOM    // User-defined comparator
```

---

## Usage Patterns

### Pattern 1: Compare Two Functions
**Use case**: Testing your implementation vs standard library

```c
// Test a single case
t_test_case test = {
    .description = "Test digit '5'",
    .params = {(void*)(long)'5'},
    .param_count = 1,
    .compare_type = CMP_BOOL,
    .data_size = sizeof(int)
};

run_comparison_test(suite, "isdigit test", 
    (void*)my_isdigit, (void*)isdigit, &test);
```

### Pattern 2: Batch Function Testing
**Use case**: Test multiple inputs at once

```c
t_test_case cases[] = {
    {.params = {(void*)(long)'0'}, .param_count = 1, .compare_type = CMP_BOOL},
    {.params = {(void*)(long)'5'}, .param_count = 1, .compare_type = CMP_BOOL},
    {.params = {(void*)(long)'a'}, .param_count = 1, .compare_type = CMP_BOOL}
};

run_test_batch(suite, "isdigit batch", 
    (void*)my_isdigit, (void*)isdigit, cases, 3);
```

### Pattern 3: Edge Case Testing (No Reference)
**Use case**: Validate behavior without a reference function

```c
bool is_positive(void *result) {
    return *(int*)result > 0;
}

t_edge_case edge = {
    .description = "abs(-5) should be positive",
    .params = {(void*)(long)-5},
    .param_count = 1,
    .validator = is_positive
};

run_edge_case_test(suite, "abs edge case", (void*)my_abs, &edge);
```

### Pattern 4: Direct Variable Comparison
**Use case**: Compare variables without calling functions

```c
int x = 42;
int y = 42;

// Method 1: Using helper function
run_variable_test(suite, "x == y", &x, &y, CMP_INT, sizeof(int));

// Method 2: Using macro
ASSERT_EQ(suite, x, y, CMP_INT);
ASSERT_TRUE(suite, x == y);
ASSERT_FALSE(suite, x != y);
```

### Pattern 5: Multi-Parameter Functions
**Use case**: Functions with 2-4 parameters

```c
// Testing: int memcmp(void *s1, void *s2, size_t n)
char buf1[] = "Hello";
char buf2[] = "Hello";

t_test_case test = {
    .params = {buf1, buf2, (void*)(long)5},
    .param_count = 3,
    .compare_type = CMP_INT,
    .data_size = sizeof(int)
};

run_comparison_test(suite, "memcmp test", 
    (void*)my_memcmp, (void*)memcmp, &test);
```

### Pattern 6: Custom Comparator
**Use case**: Special comparison logic

```c
int compare_ignore_case(const void *a, const void *b, size_t size) {
    return strcasecmp((const char*)a, (const char*)b);
}

t_test_case test = {
    .compare_type = CMP_CUSTOM,
    .custom_cmp = compare_ignore_case,
    // ... other fields
};
```

### Pattern 7: Memory Block Comparison
**Use case**: Compare structs or arrays

```c
struct point { int x; int y; };
struct point p1 = {10, 20};
struct point p2 = {10, 20};

run_variable_test(suite, "p1 == p2", 
    &p1, &p2, CMP_MEMORY, sizeof(struct point));
```

---

## Common Validators

Pre-built validators for edge case testing:

```c
validate_non_null()  // Result is not NULL
validate_null()      // Result is NULL
validate_positive()  // Result > 0
validate_negative()  // Result < 0
validate_zero()      // Result == 0
```

Create custom validators:
```c
bool is_even(void *result) {
    return (*(int*)result % 2) == 0;
}

bool in_range(void *result) {
    int val = *(int*)result;
    return val >= 0 && val <= 100;
}
```

---

## Reporting

```c
// Summary only (pass/fail counts)
test_suite_print_summary(suite);

// Detailed (shows each test)
test_suite_print_detailed(suite);
```

---

## Complete Example Template

```c
#include "test_framework.h"

int my_function(int x) {
    return x * 2;
}

int main(void)
{
    // 1. Create suite
    t_test_suite *suite = TEST_SUITE("My tests");
    
    // 2. Define test cases
    t_test_case cases[] = {
        {
            .description = "Test with 5",
            .params = {(void*)(long)5},
            .param_count = 1,
            .compare_type = CMP_INT,
            .data_size = sizeof(int)
        }
    };
    
    // 3. Run tests
    run_test_batch(suite, "my_function", 
        (void*)my_function, (void*)reference_function, cases, 1);
    
    // 4. Report results
    test_suite_print_summary(suite);
    
    // 5. Cleanup
    test_suite_destroy(suite);
    
    return 0;
}
```

---

## Tips & Best Practices

1. **For ctype-style functions**: Use `CMP_BOOL` to compare truthiness
2. **For floating-point**: Framework includes epsilon comparison automatically
3. **For strings**: Use `CMP_STRING` (uses strcmp internally)
4. **For memory blocks**: Use `CMP_MEMORY` and specify exact size
5. **Edge cases**: When you don't have a reference, use validators
6. **Batch testing**: Group related tests for cleaner output
7. **Custom logic**: Implement custom comparators for special cases

---

## Integration with Your Project

**Minimal changes needed**:
1. Include `test_framework.h`
2. Replace manual loops with `run_test_batch()`
3. Use `test_suite_print_summary()` for reports
4. Remove manual counter tracking

**Your current test** → **New framework**:
```c
// OLD: Manual loop
for (i = 0; i < AMAX; ++i) {
    int result1 = ft(i);
    int result2 = std_table[ref_index](i);
    // manual comparison...
}

// NEW: Automated batch
t_test_case cases[AMAX];
for (i = 0; i < AMAX; i++) {
    cases[i] = (t_test_case){
        .params = {(void*)(long)i},
        .param_count = 1,
        .compare_type = CMP_BOOL
    };
}
run_test_batch(suite, "function", ft, std_fn, cases, AMAX);
```

---

## Key Advantages

✅ **Flexible**: 0-4 parameters, any type  
✅ **Reusable**: One framework for all tests  
✅ **Readable**: Clear test structure  
✅ **Maintainable**: Easy to add new tests  
✅ **Informative**: Detailed reporting  
✅ **Extensible**: Custom comparators & validators