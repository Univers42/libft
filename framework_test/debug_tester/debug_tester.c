#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../debug/ft_debug.h"
#include "../framework.h"
#include "../../stdio/ft_stdio.h"

#define MAX_TESTS 50
#define MAX_TEST_DATA_SIZE 256

// Test categories
typedef enum {
    TEST_ASCII = 0,
    TEST_HEX,
    TEST_MEMORY,
    // Add new categories here
    TEST_CATEGORY_COUNT
} test_category_t;

// Test data types
typedef enum {
    DATA_TEXT,        // Text string
    DATA_BINARY,      // Binary/special characters
    DATA_PATTERN,     // Generated pattern (0-255)
    DATA_EMPTY        // Empty data (size 0)
} data_type_t;

// Test expectation types
typedef enum {
    EXPECT_VISUAL,    // Visual verification by user
    EXPECT_AUTOMATIC  // Automatic verification (future)
} expect_type_t;

// Test data structure
typedef struct {
    data_type_t type;
    unsigned char data[MAX_TEST_DATA_SIZE];
    size_t size;
    unsigned long address;
} test_data_t;

// Test case structure
typedef struct {
    const char *name;
    const char *description;
    test_category_t category;
    data_type_t data_type;
    size_t data_size;
    void (*setup_func)(test_data_t*);
    void (*test_func)(test_data_t*);
    expect_type_t expect_type;
    const char *expect_description;
} test_case_t;

// Test registry
static test_case_t g_tests[MAX_TESTS];
static int g_test_count = 0;

// Test counters
static int test_run_count = 0;
static int pass_count = 0;

// Function to mark test result
void mark_test(const char *test_name, int result) {
    test_run_count++;
    if (result) {
        printf(GREEN "[PASS] %s\n" RESET, test_name);
        pass_count++;
    } else {
        printf(RED "[FAIL] %s\n" RESET, test_name);
    }
}

// Helper function to get user verification
int get_user_verification(const char *prompt) {
    char response;
    printf(YELLOW "%s (y/n): " RESET, prompt);
    scanf(" %c", &response);
    return (response == 'y' || response == 'Y');
}

// Register a test
void register_test(const char *name, const char *description, 
                  test_category_t category, data_type_t data_type, size_t data_size,
                  void (*setup_func)(test_data_t*), void (*test_func)(test_data_t*),
                  expect_type_t expect_type, const char *expect_description) {
    if (g_test_count < MAX_TESTS) {
        g_tests[g_test_count].name = name;
        g_tests[g_test_count].description = description;
        g_tests[g_test_count].category = category;
        g_tests[g_test_count].data_type = data_type;
        g_tests[g_test_count].data_size = data_size;
        g_tests[g_test_count].setup_func = setup_func;
        g_tests[g_test_count].test_func = test_func;
        g_tests[g_test_count].expect_type = expect_type;
        g_tests[g_test_count].expect_description = expect_description;
        g_test_count++;
    }
}

// Data setup functions
void setup_normal_text(test_data_t *test_data) {
    const char *text = "Hello, World!";
    strncpy((char *)test_data->data, text, MAX_TEST_DATA_SIZE - 1);
    test_data->size = strlen(text);
    test_data->address = (unsigned long)test_data->data;
}

void setup_non_printable(test_data_t *test_data) {
    unsigned char buffer[] = {'H', 'e', 'l', 'l', 'o', '\0', '\n', '\t', '\r', 0x7F};
    memcpy(test_data->data, buffer, sizeof(buffer));
    test_data->size = sizeof(buffer);
    test_data->address = (unsigned long)test_data->data;
}

void setup_binary_data(test_data_t *test_data) {
    unsigned char buffer[] = {0x00, 0xFF, 0x0F, 0xF0, 0x55, 0xAA, 0x7F, 0x80};
    memcpy(test_data->data, buffer, sizeof(buffer));
    test_data->size = sizeof(buffer);
    test_data->address = (unsigned long)test_data->data;
}

void setup_pattern_data(test_data_t *test_data) {
    size_t size = test_data->size;
    for (size_t i = 0; i < size && i < MAX_TEST_DATA_SIZE; i++) {
        test_data->data[i] = i % 256;
    }
    test_data->address = (unsigned long)test_data->data;
}

void setup_large_data(test_data_t *test_data) {
    size_t size = test_data->size;
    for (size_t i = 0; i < size && i < MAX_TEST_DATA_SIZE; i++) {
        test_data->data[i] = i % 256;
    }
    test_data->address = (unsigned long)test_data->data;
}

void setup_empty_data(test_data_t *test_data) {
    strncpy((char *)test_data->data, "This shouldn't be printed", MAX_TEST_DATA_SIZE - 1);
    test_data->size = 0; // Set size to 0
    test_data->address = (unsigned long)test_data->data;
}

// Test functions
void test_print_ascii_func(test_data_t *test_data) {
    ft_print_ascii(test_data->data, test_data->size);
    printf("\n");
}

void test_print_hex_func(test_data_t *test_data) {
    ft_print_hex(test_data->data, test_data->size, test_data->address);
    printf("\n");
}

void test_print_memory_func(test_data_t *test_data) {
    ft_print_memory(test_data->data, test_data->size);
}

// Run a specific test
void run_test(test_case_t *test) {
    test_data_t test_data;
    memset(&test_data, 0, sizeof(test_data_t));
    test_data.size = test->data_size;
    
    printf("\n--- Testing %s ---\n", test->name);
    printf("Description: %s\n", test->description);
    
    // Setup test data
    if (test->setup_func) {
        test->setup_func(&test_data);
    }
    
    // Display expectations
    printf("Expected: %s\n", test->expect_description);
    printf("Actual:   ");
    
    // Run the test
    if (test->test_func) {
        test->test_func(&test_data);
    }
    
    // Verify result
    if (test->expect_type == EXPECT_VISUAL) {
        mark_test(test->name, get_user_verification("Does the output match the expected?"));
    }
}

// Run tests by category
void run_tests_by_category(test_category_t category) {
    char *category_names[] = {
        "ft_print_ascii",
        "ft_print_hex",
        "ft_print_memory"
        // Add new category names here
    };
    
    print_header(category_names[category]);
    
    for (int i = 0; i < g_test_count; i++) {
        if (g_tests[i].category == category) {
            run_test(&g_tests[i]);
        }
    }
}

// Initialize all tests
void init_tests() {
    // ASCII Tests
    register_test(
        "ASCII - Normal Text",
        "Tests printing ASCII representation of standard text",
        TEST_ASCII, DATA_TEXT, 13,
        setup_normal_text, test_print_ascii_func, EXPECT_VISUAL,
        "Hello, World!"
    );
    
    register_test(
        "ASCII - Non-Printable Characters",
        "Tests printing ASCII representation of non-printable characters",
        TEST_ASCII, DATA_BINARY, 10,
        setup_non_printable, test_print_ascii_func, EXPECT_VISUAL,
        "Hello....."
    );
    
    // HEX Tests
    register_test(
        "HEX - Simple Text",
        "Tests printing hexadecimal representation of text",
        TEST_HEX, DATA_TEXT, 5,
        setup_normal_text, test_print_hex_func, EXPECT_VISUAL,
        "<address>: 4865 6c6c 6f ..."
    );
    
    register_test(
        "HEX - Binary Data",
        "Tests printing hexadecimal representation of binary data",
        TEST_HEX, DATA_BINARY, 8,
        setup_binary_data, test_print_hex_func, EXPECT_VISUAL,
        "<address>: 00ff 0ff0 55aa 7f80 ..."
    );
    
    // MEMORY Tests
    register_test(
        "MEMORY - Text",
        "Tests printing memory dump of text",
        TEST_MEMORY, DATA_TEXT, 13,
        setup_normal_text, test_print_memory_func, EXPECT_VISUAL,
        "Memory dump of 'Hello, World!'"
    );
    
    register_test(
        "MEMORY - Binary Pattern",
        "Tests printing memory dump of a binary pattern",
        TEST_MEMORY, DATA_PATTERN, 32,
        setup_pattern_data, test_print_memory_func, EXPECT_VISUAL,
        "Memory dump showing 2 rows of 16 bytes (0-31) with ASCII representation"
    );
    
    register_test(
        "MEMORY - Empty",
        "Tests printing memory dump with size 0",
        TEST_MEMORY, DATA_EMPTY, 0,
        setup_empty_data, test_print_memory_func, EXPECT_VISUAL,
        "No output (empty)"
    );
    
    register_test(
        "MEMORY - Large Data",
        "Tests printing memory dump of large data set",
        TEST_MEMORY, DATA_PATTERN, 100,
        setup_large_data, test_print_memory_func, EXPECT_VISUAL,
        "Memory dump of 100 bytes"
    );
    
    // Add more tests here by calling register_test
}

// Main function
int main() {
    print_header("DEBUGGING FUNCTIONS TESTER");
    
    // Initialize the test registry
    init_tests();
    
    // Run tests by category
    for (int category = 0; category < TEST_CATEGORY_COUNT; category++) {
        run_tests_by_category(category);
    }
    
    // Print summary
    print_header("TEST SUMMARY");
    printf("Tests passed: %d/%d (%.1f%%)\n", 
           pass_count, test_run_count, 
           (test_run_count > 0 ? (float)pass_count / test_run_count * 100 : 0));
    
    return 0;
}
