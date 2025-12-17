/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ternary.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 14:56:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 15:23:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // added for intptr_t
#include <stdbool.h>

// /* Add macros and prototypes so they're visible before use */
// #define VSTR_FIXED 0x01
// #define VUNSET 0x02

// void *set_vareq(const char *name, void *arg2, int flags);
// void *some_func(void);

/* Single universal ternary function using void pointers */

/* For function execution: ternary(cond, fn1, fn2) */
void	*ternary(bool cond, void *true_val, void *false_val)
{
	if (cond)
		return (true_val);
	else
		return (false_val);
}

// /* For function calls that need execution: ternary_call(cond, fn1, fn2) */
// typedef void *(*ternary_fn_t)(void);

// static inline void *ternary_call(int cond, ternary_fn_t true_fn, ternary_fn_t false_fn)
// {
// 	if (cond)
// 	{
// 		return true_fn();
// 	}
// 	else
// 	{
// 		return false_fn();
// 	}
// }

// /* Usage Examples */

// /* Example 1: Simple value selection */
// void example_values(void)
// {
// 	int cond = 1;
// 	char *result;
// 	int *num_ptr;
// 	void *vp;

// 	/* Select between two strings */
// 	result = (char *)ternary(cond, (void *)"true_string", (void *)"false_string");

// 	/* Select between two pointers */
// 	int a = 10, b = 20;
// 	num_ptr = (int *)ternary(cond, (void *)&a, (void *)&b);

// 	/* Select between value and NULL */
// 	vp = ternary(cond, (void *)0x12345, NULL);
// }

// /* Example 1b: Complex conditions - ANY expression works! */
// void example_complex_conditions(void)
// {
// 	int a = 5, b = 10, c = 15;
// 	int flags = 0x04;
// 	char *result;
// 	void *vp;

// 	/* Comparison operators */
// 	result = (char *)ternary(a == b, (void *)"equal", (void *)"not equal");
// 	result = (char *)ternary(a < b, (void *)"less", (void *)"greater or equal");
// 	result = (char *)ternary(a > b, (void *)"greater", (void *)"less or equal");
// 	result = (char *)ternary(a != b, (void *)"different", (void *)"same");

// 	/* Complex boolean expressions */
// 	result = (char *)ternary(a > b && b < c, (void *)"both true", (void *)"at least one false");
// 	result = (char *)ternary(a == 5 || b == 20, (void *)"at least one true", (void *)"both false");

// 	/* Bitwise operations in condition */
// 	result = (char *)ternary(flags & 0x04, (void *)"flag set", (void *)"flag not set");
// 	result = (char *)ternary((flags & VUNSET) == 0, (void *)"not unset", (void *)"is unset");

// 	/* Combined complex conditions */
// 	vp = ternary((a > b && (flags & 0x04)) || c == VUNSET,
// 				 (void *)some_func(),
// 				 NULL);

// 	/* Negation */
// 	result = (char *)ternary(!a, (void *)"a is zero", (void *)"a is non-zero");
// 	result = (char *)ternary(!(flags & VUNSET), (void *)"set", (void *)"unset");
// }

// /* Example 2: Your original case - direct approach */
// void example_your_case(int eq, const char *name, int flags)
// {
// 	void *vp;

// 	/* Before: 4 lines (fixed to correct prototype usage) */
// 	if (eq)
// 		vp = set_vareq(name, (void *)(intptr_t)(VSTR_FIXED | flags), VSTR_FIXED | flags);
// 	else
// 		vp = set_vareq(name, NULL, VSTR_FIXED | flags);

// 	/* After: Using ternary for the varying argument, then call */
// 	void *arg2 = ternary(eq, (void *)(intptr_t)(VSTR_FIXED | flags), NULL);
// 	vp = set_vareq(name, arg2, VSTR_FIXED | flags);

// 	/* Or even more compact - inline: */
// 	vp = set_vareq(name, ternary(eq, (void *)(intptr_t)(VSTR_FIXED | flags), NULL), VSTR_FIXED | flags);

// 	/* With complex conditions: */
// 	vp = set_vareq(name,
// 				   ternary(eq && (flags & VUNSET) == 0, (void *)(intptr_t)(VSTR_FIXED | flags), NULL),
// 				   VSTR_FIXED | flags);
// }

// /* Example 3: Function pointers */
// void func_a(void) { /* ... */ }
// void func_b(void) { /* ... */ }

// void example_function_selection(int cond)
// {
// 	void (*selected_func)(void);

// 	/* Select which function to call */
// 	selected_func = (void (*)(void))ternary(cond, (void *)func_a, (void *)func_b);
// 	selected_func();
// }

// /* Example 4: Assignment variations */
// void example_assignments(int cond)
// {
// 	int x;
// 	char *str;

// 	/* Assign different values based on condition */
// 	x = (int)(long)ternary(cond, (void *)100, (void *)200);

// 	str = (char *)ternary(cond, (void *)"yes", (void *)"no");
// }

// /* Example 5: Complex expressions */
// typedef struct
// {
// 	int value;
// 	char name[32];
// } data_t;

// void example_complex(int cond, data_t *d1, data_t *d2)
// {
// 	data_t *selected;

// 	/* Select entire structure */
// 	selected = (data_t *)ternary(cond, (void *)d1, (void *)d2);

// 	/* Use selected structure */
// 	selected->value = 42;
// }

// /* Example 6: Inline usage */
// void example_inline(int eq, const char *name, int flags)
// {
// 	void *vp;

// 	/* Ultra-compact version */
// 	vp = set_vareq(name,
// 				   ternary(eq, (void *)(intptr_t)(VSTR_FIXED | flags), NULL),
// 				   VSTR_FIXED | flags);
// }

// /* Mock functions for demonstration */
// void *set_vareq(const char *name, void *arg2, int flags)
// {
// 	printf("set_vareq called: name=%s, arg2=%p, flags=0x%x\n", name, arg2, flags);
// 	return (void *)0xDEADBEEF;
// }

// void *some_func(void)
// {
// 	return (void *)0x12345;
// }

// /* Main function to test everything */
// int main(void)
// {
// 	printf("=== Testing ternary function ===\n\n");

// 	/* Test 1: Simple comparisons */
// 	printf("Test 1: Simple comparisons\n");
// 	int a = 5, b = 10;
// 	char *result;

// 	result = (char *)ternary(a == b, (void *)"equal", (void *)"not equal");
// 	printf("  a == b: %s\n", result);

// 	result = (char *)ternary(a < b, (void *)"less", (void *)"greater or equal");
// 	printf("  a < b: %s\n", result);

// 	result = (char *)ternary(a > b, (void *)"greater", (void *)"less or equal");
// 	printf("  a > b: %s\n\n", result);

// 	/* Test 2: Complex boolean expressions */
// 	printf("Test 2: Complex conditions\n");
// 	int c = 15;
// 	result = (char *)ternary(a < b && b < c, (void *)"both true", (void *)"at least one false");
// 	printf("  a < b && b < c: %s\n", result);

// 	result = (char *)ternary(a > b || c > b, (void *)"at least one true", (void *)"both false");
// 	printf("  a > b || c > b: %s\n\n", result);

// 	/* Test 3: Bitwise operations */
// 	printf("Test 3: Bitwise operations\n");
// 	int flags = 0x04;
// 	result = (char *)ternary(flags & 0x04, (void *)"flag set", (void *)"flag not set");
// 	printf("  flags & 0x04: %s\n", result);

// 	flags = 0x01;
// 	result = (char *)ternary(flags & 0x04, (void *)"flag set", (void *)"flag not set");
// 	printf("  flags & 0x04 (when flags=0x01): %s\n\n", result);

// 	/* Test 4: Your original use case */
// 	printf("Test 4: Original use case (set_vareq)\n");
// 	void *vp;
// 	int eq = 1;
// 	const char *name = "test_var";
// 	flags = VSTR_FIXED;

// 	printf("  With eq=1:\n");
// 	vp = set_vareq(name, ternary(eq, (void *)(intptr_t)(VSTR_FIXED | flags), NULL), VSTR_FIXED | flags);
// 	printf("  Returned: %p\n", vp);

// 	eq = 0;
// 	printf("  With eq=0:\n");
// 	vp = set_vareq(name, ternary(eq, (void *)(intptr_t)(VSTR_FIXED | flags), NULL), VSTR_FIXED | flags);
// 	printf("  Returned: %p\n\n", vp);

// 	/* Test 5: Pointer selection */
// 	printf("Test 5: Pointer selection\n");
// 	int x = 100, y = 200;
// 	int *selected = (int *)ternary(a < b, (void *)&x, (void *)&y);
// 	printf("  Selected pointer points to: %d\n\n", *selected);

// 	/* Test 6: Negation and complex conditions */
// 	printf("Test 6: Negation and complex conditions\n");
// 	result = (char *)ternary(!0, (void *)"not zero", (void *)"zero");
// 	printf("  !0: %s\n", result);

// 	result = (char *)ternary(!(flags & VUNSET), (void *)"VUNSET not set", (void *)"VUNSET set");
// 	printf("  !(flags & VUNSET): %s\n\n", result);

// 	/* Test 7: NULL handling */
// 	printf("Test 7: NULL handling\n");
// 	void *ptr = ternary(0, (void *)0x12345, NULL);
// 	printf("  Selected pointer (should be NULL): %p\n", ptr);

// 	ptr = ternary(1, (void *)0x12345, NULL);
// 	printf("  Selected pointer (should be 0x12345): %p\n", ptr);

// 	printf("\n=== All tests completed ===\n");
// 	return 0;
// }
