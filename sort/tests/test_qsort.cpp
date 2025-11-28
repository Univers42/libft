/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_qsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 22:11:51 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 00:41:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "qsort.h"

/* ----------------------------- Test main ----------------------------- */
static void *dup_mem(const void *src, size_t n, size_t size)
{
	void *dst;

	if (n == 0)
		return (NULL);
	dst = malloc(n * size);
	if (!dst)
		return (NULL);
	memcpy(dst, src, n * size);
	return (dst);
}

/* comparator for stdlib qsort (ints) */
static int cmp_int_qsort(const void *a, const void *b)
{
	int ia;
	int ib;

	ia = *(const int *)a;
	ib = *(const int *)b;
	if (ia < ib)
		return (-1);
	if (ia > ib)
		return (1);
	return (0);
}

/* wrapper comparator for ft_qsort (ints) */
static int int_cmp(const void *a, const void *b)
{
	return (cmp_int_qsort(a, b));
}

/* run one integer test case: compare ft_qsort output to stdlib qsort */
static int run_int_case(const char *name, int *arr, size_t n)
{
	int *work;
	int *expected;
	int ok;

	work = NULL;
	expected = NULL;
	if (n > 0)
	{
		work = dup_mem(arr, n, sizeof(int));
		expected = dup_mem(arr, n, sizeof(int));
		if (!work || !expected)
		{
			free(work);
			free(expected);
			printf("%s: MALLOC FAIL\n", name);
			return (0);
		}
	}
	/* call ft_qsort */
	ft_qsort(work, n, sizeof(int), int_cmp);
	/* build expected using stdlib qsort */
	if (expected)
		qsort(expected, n, sizeof(int), cmp_int_qsort);
	/* compare results */
	if (n == 0)
		ok = 1;
	else
		ok = (memcmp(work, expected, n * sizeof(int)) == 0);
	printf("%s: %s (n=%zu)\n", name, ok ? "PASS" : "FAIL", n);
	free(work);
	free(expected);
	return (ok);
}

/* helper to print small arrays */
static void print_small_ints(const int *a, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		printf("%d ", a[i]);
		i++;
	}
	printf("\n");
}

/* test sorting of structs (two-field key) */
typedef struct s_pair
{
	int primary;
	int secondary;
} t_pair;

static int pair_cmp(const void *pa, const void *pb)
{
	const t_pair *a;
	const t_pair *b;

	a = pa;
	b = pb;
	if (a->primary < b->primary)
		return (-1);
	if (a->primary > b->primary)
		return (1);
	/* tie break on secondary */
	if (a->secondary < b->secondary)
		return (-1);
	if (a->secondary > b->secondary)
		return (1);
	return (0);
}

/* wrapper to satisfy ft_qsort signature for pairs */
static int pair_cmp_wrap(const void *pa, const void *pb)
{
	return (pair_cmp(pa, pb));
}

/* run pair test comparing ft_qsort vs stdlib qsort */
static int run_pair_case(const char *name, t_pair *arr, size_t n)
{
	t_pair *work;
	t_pair *expected;
	int ok;

	work = dup_mem(arr, n, sizeof(t_pair));
	expected = dup_mem(arr, n, sizeof(t_pair));
	if (!work || !expected)
	{
		free(work);
		free(expected);
		printf("%s: MALLOC FAIL\n", name);
		return (0);
	}
	ft_qsort(work, n, sizeof(t_pair), pair_cmp_wrap);
	qsort(expected, n, sizeof(t_pair), pair_cmp);
	ok = (memcmp(work, expected, n * sizeof(t_pair)) == 0);
	printf("%s: %s (n=%zu)\n", name, ok ? "PASS" : "FAIL", n);
	free(work);
	free(expected);
	return (ok);
}

int main(void)
{
	int pass = 0;
	int total = 0;
	int *a0 = NULL;
	int a1[] = {42};
	int a2[] = {2, 1};
	int a3[] = {1, 1, 1, 1};
	int a4[] = {1, 2, 3, 4, 5};
	int a5[] = {5, 4, 3, 2, 1};
	int a6[] = {3, 1, 4, 1, 5, 9, 2, 6, 5};
	int i;
	int nrand = 1000;
	int *rand_arr;

	/* simple edge cases */
	total++;
	pass += run_int_case("empty", a0, 0);
	total++;
	pass += run_int_case("single", a1, 1);
	total++;
	pass += run_int_case("two elems", a2, 2);
	total++;
	pass += run_int_case("all equal", a3, 4);
	total++;
	pass += run_int_case("already sorted", a4, 5);
	total++;
	pass += run_int_case("reverse sorted", a5, 5);
	total++;
	pass += run_int_case("mixed small", a6, 9);

	/* many duplicates */
	{
		int dup[50];
		for (i = 0; i < 50; i++)
			dup[i] = (i % 3); /* values 0,1,2 repeated */
		total++;
		pass += run_int_case("many duplicates", dup, 50);
	}

	/* random data */
	srand(0);
	rand_arr = malloc(nrand * sizeof(int));
	if (rand_arr)
	{
		for (i = 0; i < nrand; i++)
			rand_arr[i] = rand() % 1000 - 500;
		total++;
		pass += run_int_case("random 1000", rand_arr, nrand);
		free(rand_arr);
	}
	else
	{
		printf("random test skipped (malloc fail)\n");
	}

	/* small arrays printed for visual verification */
	{
		int small[] = {7, 3, 5, 2, 9, 1};
		int *wrk = dup_mem(small, 6, sizeof(int));
		printf("\nvisual check before: ");
		print_small_ints(small, 6);
		ft_qsort(wrk, 6, sizeof(int), int_cmp);
		printf("visual check after : ");
		print_small_ints(wrk, 6);
		free(wrk);
	}

	/* struct sorting */
	{
		t_pair pairs[] = {
			{2, 10}, {1, 5}, {2, 5}, {1, 1}, {3, 0}, {2, 10}};
		total++;
		pass += run_pair_case("pairs", pairs, sizeof(pairs) / sizeof(pairs[0]));
	}

	/* stress / stack-depth related test: sorted sequence (worst-case pivot) */
	{
		size_t m = 200;
		int *arr = malloc(m * sizeof(int));
		if (arr)
		{
			for (i = 0; (size_t)i < m; i++)
				arr[i] = (int)i; /* already sorted */
			total++;
			pass += run_int_case("sorted 200 (stress)", arr, m);
			free(arr);
		}
		else
		{
			printf("sorted 200 skipped (malloc fail)\n");
		}
	}

	printf("\nSummary: %d/%d tests passed\n", pass, total);
	return (pass == total ? 0 : 1);
}