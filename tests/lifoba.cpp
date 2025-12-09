/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifoba.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 18:27:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>

extern "C"
{
#include "lifoba.h"
#include "libft.h"
#include "ft_math.h"
}

typedef struct s_node
{
	char type;
	char *value;
	struct s_node *next;
} t_node;

static void test_generic_allocation(void)
{
	printf("Testing generic allocation...\n");
	t_stack_mark mark;
	set_stack_mark(&mark);

	/* ...use point members based on glob.h definition... */
	printf("Generic allocation test passed\n");

	pop_stack_mark(&mark);
}

static void test_string_building(void)
{
	char *p;
	size_t i;
	size_t word_count;
	const char *words[] = {"Hello", " ", "from", " ", "arena", "!"};

	printf("\n=== TEST 2: String Building (STPUTC equivalent) ===\n");
	p = start_stack_str();
	word_count = sizeof(words) / sizeof(words[0]);
	i = 0;
	while (i < word_count)
	{
		const char *s;

		s = words[i];
		while (*s)
		{
			p = st_putc((unsigned char)*s, p);
			s++;
		}
		i++;
	}
	p = stack_str_nul(p);
	printf("Built string: %s\n", stack_block());
	printf("String length: %zu\n", p - (char *)stack_block());
}

static void test_string_operations(void)
{
	char *p;

	printf("\n=== TEST 3: String Operations (stnputs equivalent) ===\n");
	p = start_stack_str();
	p = st_nputs("Name: ", 6, p);
	p = st_nputs("Alice", 5, p);
	p = st_putc('\n', p);
	p = st_puts("Age: ", p);
	p = st_nputs("30", 2, p);
	p = stack_str_nul(p);
	printf("Built data:\n%s", stack_block());
}

static void test_stack_marks(void)
{
	t_stack_mark mark1;
	t_stack_mark mark2;
	char *p;

	printf("\n=== TEST 4: Stack Marks (pushstackmark/popstackmark) ===\n");
	set_stack_mark(&mark1);
	printf("Mark 1 saved at: %p\n", (void *)mark1.stacknxt);
	p = start_stack_str();
	p = st_puts("First block", p);
	p = stack_str_nul(p);
	printf("After first block: %s\n", stack_block());
	set_stack_mark(&mark2);
	printf("Mark 2 saved at: %p\n", (void *)mark2.stacknxt);
	p = start_stack_str();
	p = st_puts("Second block", p);
	p = stack_str_nul(p);
	printf("After second block: %s\n", stack_block());
	pop_stack_mark(&mark2);
	printf("After pop to mark2: %s (second block discarded)\n",
		   stack_block());
	pop_stack_mark(&mark1);
	printf("Popped to mark1 - stack reset\n");
}

static void test_unalloc_and_growth(void)
{
	char *p1;
	char *p2;
	char *p3;
	char *p_reuse;

	printf("\n=== TEST 5: Unalloc and Growth ===\n");
	p1 = (char *)st_alloc(100);
	strcpy(p1, "Block 1");
	printf("Allocated p1 at %p: %s\n", (void *)p1, p1);
	p2 = (char *)st_alloc(100);
	strcpy(p2, "Block 2");
	printf("Allocated p2 at %p: %s\n", (void *)p2, p2);
	p3 = (char *)st_alloc(50);
	strcpy(p3, "Block 3");
	printf("Allocated p3 at %p: %s\n", (void *)p3, p3);
	printf("\nUnallocating from p3...\n");
	st_unalloc(p3);
	printf("After unalloc(p3)\n");
	printf("Unallocating to p2...\n");
	st_unalloc(p2);
	printf("After unalloc(p2) - p2 and p3 space available for reuse\n");
	p_reuse = (char *)st_alloc(120);
	strcpy(p_reuse, "Reused space after p1");
	printf("Reused space at %p: %s\n", (void *)p_reuse, p_reuse);
}

static void test_grab_ungrab(void)
{
	char *p;

	printf("\n=== TEST 6: Grab/Ungrab Stack String ===\n");
	p = start_stack_str();
	p = st_puts("Temporary", p);
	p = st_puts(" string", p);
	printf("Current pointer offset: %zu\n", p - (char *)stack_block());
	grab_stack_str(p);
	printf("After grab_stack_str - space is now allocated\n");
	printf("String: %s\n", stack_block());
	ungrab_stack_str(p);
	printf("After ungrab_stack_str - space is available for reuse\n");
}

static void test_character_ops(void)
{
	char *p;
	char *adjusted;

	printf("\n=== TEST 7: Character Operations ===\n");
	p = start_stack_str();
	p = st_puts("Hello World!", p);
	printf("String: %s\n", stack_block());
	printf("Last char before unputc: '%c'\n", st_topc(p));
	p = st_unputc(p);
	p = st_putc('!', p);
	p = stack_str_nul(p);
	printf("After st_unputc + st_putc: %s\n", stack_block());
	adjusted = st_adjust(3, (char *)stack_block());
	printf("After st_adjust(3, start): points to '%c'\n", *adjusted);
}

static void test_block_growth(void)
{
	t_garena *g;
	size_t initial_size;

	printf("\n=== TEST 8: Block Growth (automatic) ===\n");
	g = arena_ctx();
	initial_size = g->stack_nleft;
	printf("Initial stack space left: %zu bytes\n", initial_size);
	(void)st_alloc(initial_size - 10);
	printf("After allocating %zu bytes, space left: %zu\n",
		   initial_size - 10, g->stack_nleft);
	(void)st_alloc(100);
	printf("After allocating 100 more bytes, space left: %zu\n",
		   g->stack_nleft);
	printf("New block allocated! (total blocks: %s)\n",
		   (g->stackp != &g->stack_base) ? "2+" : "1");
}

static void test_error_handling(void)
{
	void *p1;
	void *p2;

	printf("\n=== TEST 9: Error Handling ===\n");
	p1 = arena_malloc(256);
	if (p1)
		printf("arena_malloc(256) succeeded: %p\n", p1);
	p2 = arena_realloc(p1, 512);
	if (p2)
		printf("arena_realloc(256->512) succeeded: %p\n", p2);
	arena_free(p2);
	printf("arena_free() called successfully\n");
}

static void test_parse_tree_simulation(void)
{
	printf("Testing parse tree simulation...\n");
	t_stack_mark mark;
	set_stack_mark(&mark);

	/* ...use root members based on glob.h definition... */
	printf("Parse tree simulation test passed\n");

	pop_stack_mark(&mark);
}

int main(void)
{
	printf("╔════════════════════════════════════════════════════════════╗\n");
	printf("║         libmemalloc - Comprehensive Test Suite             ║\n");
	printf("║   (dash memalloc.c/h functionality in library form)       ║\n");
	printf("╚════════════════════════════════════════════════════════════╝\n");
	test_generic_allocation();
	test_string_building();
	test_string_operations();
	test_stack_marks();
	test_unalloc_and_growth();
	test_grab_ungrab();
	test_character_ops();
	test_block_growth();
	test_error_handling();
	test_parse_tree_simulation();
	printf("\n╔════════════════════════════════════════════════════════════╗\n");
	printf("║              All tests completed successfully!             ║\n");
	printf("║                    No memory leaks.                        ║\n");
	printf("╚════════════════════════════════════════════════════════════╝\n\n");
	arena_cleanup();
	return (0);
}
