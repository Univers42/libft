/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctype_tester.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/28 00:01:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../framework.h"

/**
 * Tests a ft_ function against its standard library counterpart
 * 
 * @param ft_func The function to test
 * @param std_func The standard library function to compare against
 * @param name The name of the function (for display)
 * @return Number of errors found (0 if all tests pass)
 */
int test_ctype_function(t_ctype_func ft_func, t_ctype_func std_func, const char *name)
{
    int errors = 0;
    int mismatch_values[10];
    int mismatch_count = 0;
    
    /* Correctness test */
    printf(BOLD "╔══════════════════════════════════════╗\n" RESET);
    printf(BOLD "║ Testing %-28s ║\n" RESET, name);
    printf(BOLD "╠══════════════════════════════════════╣\n" RESET);
    
    for (int c = -10; c < CHAR_RANGE; ++c)
    {
        int std_result = std_func(c);
        int ft_result = ft_func(c);
        if ((!!std_result) != (!!ft_result))
        {
            if (mismatch_count < 10)
                mismatch_values[mismatch_count] = c;
            mismatch_count++;
            errors++;
        }
    }
    
    if (errors == 0)
        printf("║ %s✓ All tests passed!%s                  ║\n", GREEN, RESET);
    else
    {
        printf("║ %s✗ Found %d mismatches%s                 ║\n", RED, errors, RESET);
        printf(BOLD "║ Sample mismatches:                    ║\n" RESET);
        for (int i = 0; i < mismatch_count && i < 10; i++)
        {
            int c = mismatch_values[i];
            char display_char = (c >= 32 && c < 127) ? c : ' ';
            printf("║   char: %3d ('%c') - std: %d, ft: %d     ║\n", 
                   c, display_char, std_func(c), ft_func(c));
        }
    }
    printf(BOLD "║ Benchmarking:                        ║\n" RESET);
    clock_t std_start = clock();
    volatile int std_sum = 0;
    for (int i = 0; i < BENCHMARK_ITERATIONS; ++i)
        std_sum += std_func(i % CHAR_RANGE);
    clock_t std_end = clock();
    double std_time = (double)(std_end - std_start) / CLOCKS_PER_SEC;
    clock_t ft_start = clock();
    volatile int ft_sum = 0;
    for (int i = 0; i < BENCHMARK_ITERATIONS; ++i)
        ft_sum += ft_func(i % CHAR_RANGE);
    clock_t ft_end = clock();
    double ft_time = (double)(ft_end - ft_start) / CLOCKS_PER_SEC;
    printf("║ %-12s: %s%.6f seconds%s       ║\n", "libc", BLUE, std_time, RESET);
    printf("║ %-12s: %s%.6f seconds%s       ║\n", name, BLUE, ft_time, RESET);
    double performance_ratio = std_time / ft_time;
    const char *performance_desc;
    const char *color;
    
    if (performance_ratio > 1.2) {
        performance_desc = "Faster!";
        color = GREEN;
    } else if (performance_ratio < 0.8) {
        performance_desc = "Slower";
        color = RED;
    } else {
        performance_desc = "Similar";
        color = YELLOW;
    }
    
    printf("║ Performance : %s%-12s%s (%.2fx)   ║\n", 
           color, performance_desc, RESET, performance_ratio);
    printf(BOLD "╚══════════════════════════════════════╝\n\n" RESET);
    
    return errors;
}


/**
 * Main function that runs all the tests
 */
int main(void)
{
    int total_errors = 0;
    
    print_header("CTYPE");
    
    /* Test all ctype functions */
    total_errors += test_ctype_function(ft_isalpha, isalpha, "ft_isalpha");
    total_errors += test_ctype_function(ft_isdigit, isdigit, "ft_isdigit");
    total_errors += test_ctype_function(ft_isalnum, isalnum, "ft_isalnum");
    //total_errors += test_ctype_function(ft_isascii, isascii, "ft_isascii");
    total_errors += test_ctype_function(ft_isprint, isprint, "ft_isprint");
    total_errors += test_ctype_function(ft_isspace, isspace, "ft_isspace");
    total_errors += test_ctype_function(ft_isupper, isupper, "ft_isupper");
    total_errors += test_ctype_function(ft_islower, islower, "ft_islower");
    total_errors += test_ctype_function(ft_toupper, toupper, "ft_toupper");
    total_errors += test_ctype_function(ft_tolower, tolower, "ft_tolower");
    
    /* Test extra ctype functions if available */
    #ifdef isblank
    total_errors += test_ctype_function(ft_isblank, isblank, "ft_isblank");
    #endif
    
    #ifdef isxdigit
    total_errors += test_ctype_function(ft_isxdigit, isxdigit, "ft_isxdigit");
    #endif
    
    #ifdef isgraph
    total_errors += test_ctype_function(ft_isgraph, isgraph, "ft_isgraph");
    #endif
    
    /* Print summary */
    printf("\n");
    if (total_errors == 0) {
        printf(GREEN BOLD "All tests passed successfully! 🎉\n" RESET);
    } else {
        printf(RED BOLD "Found %d errors across all tests.\n" RESET, total_errors);
    }
    printf("\n");
    
    return total_errors > 0;
}
