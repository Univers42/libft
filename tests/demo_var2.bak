/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_var.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:49:57 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 18:07:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <cstring>

extern "C"
{
#include "var.h"
#ifndef VEXPORT
#define VEXPORT 0x01
#endif
}

/* Print a small banner */
static void print_header(void)
{
    printf("libvar demo: isolated env playground\n");
    printf("-----------------------------------\n");
}

/* Test: set and lookup a string variable */
static void test_set_and_lookup(void)
{
    set_var("DEMO_FOO", "foo", 0);
    printf("DEMO_FOO -> %s\n", lookup_var("DEMO_FOO"));
}

/* Test: integer variable helpers */
static void test_set_int(void)
{
    set_varint("DEMO_NUM", 42, 0);
    printf("DEMO_NUM -> %" PRIdMAX "\n", lookup_varint("DEMO_NUM"));
}

/* Test: set_vareq using a name=value string */
static void test_set_vareq(void)
{
    char *s = strdup("DEMO_EQ=eqval");
    if (!s)
        return;
    set_vareq(s, 0); /* set_vareq takes ownership in library semantics */
    printf("DEMO_EQ -> %s\n", lookup_var("DEMO_EQ"));
}

/* Test: unset behavior */
static void test_unset(void)
{
    set_var("DEMO_DEL", "to-be-deleted", 0);
    printf("before unset DEMO_DEL -> %s\n", lookup_var("DEMO_DEL"));
    unset_var("DEMO_DEL");
    printf("after unset DEMO_DEL -> %s\n", lookup_var("DEMO_DEL") ? "still here" : "NULL (unset)");
}

/* Test: local scope push/pop demonstration */
static void test_local_scope(void)
{
    /* ensure known baseline */
    set_var("DEMO_LOCAL", "global", 0);
    printf("baseline DEMO_LOCAL -> %s\n", lookup_var("DEMO_LOCAL"));

    /* create a new local scope and shadow the variable */
    push_local_vars(1);
    set_var("DEMO_LOCAL", "scoped", 0);
    printf("inside scope DEMO_LOCAL -> %s\n", lookup_var("DEMO_LOCAL"));

    /* pop the scope and verify restoration */
    pop_local_vars();
    printf("after pop DEMO_LOCAL -> %s\n", lookup_var("DEMO_LOCAL"));
}

/* Test: export flag + listing */
static void test_export_and_list(void)
{
    /* mark a variable exported by setting with VEXPORT flag */
    set_var("DEMO_EXP", "exported", VEXPORT);
    printf("DEMO_EXP -> %s (should be exported)\n", lookup_var("DEMO_EXP"));

    /* list exported variables using list_vars (heap array, caller must free) */
    char **end = NULL;
    char **arr = list_vars(VEXPORT, 0, &end);
    if (arr)
    {
        char **it = arr;
        printf("Listing variables with VEXPORT flag:\n");
        while (it && *it)
        {
            printf("  %s\n", *it);
            it++;
        }
        free(arr);
    }

    show_vars("EXPORTS:", VEXPORT, 0);
}

int main(void)
{
    print_header();
    test_set_and_lookup();
    test_set_int();
    test_set_vareq();
    test_unset();
    test_local_scope();
    test_export_and_list();
    printf("demo complete\n");
    libvar_destroy();
    return 0;
}