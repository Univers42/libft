/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_strsub.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:43:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 20:44:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ft_string.h"

static void run_case(const char *name,
                     const char *s,
                     const char *pat,
                     const char *rep,
                     bool glob,
                     const char *expected)
{
    char *res;
    char *status;
    int ok;

    res = strsub(s, pat, rep, glob);
    if (!res)
    {
        printf("%s: NULL result\n", name);
        return;
    }
    ok = (strcmp(res, expected) == 0);
    if (ok)
        status = "PASS";
    else
        status = "FAIL";
    printf("%s: \"%s\" -> pat:\"%s\" rep:\"%s\" glob:%d\n",
           name, s, pat, rep, (int)glob);
    printf(" expected: \"%s\"\n", expected);
    printf(" got     : \"%s\" -- %s\n\n", res, status);
    free(res);
}

int main(void)
{
    run_case("test1",
             "hello world",
             "world",
             "42",
             false,
             "hello 42");
    run_case("test2",
             "aaa",
             "a",
             "b",
             true,
             "bbb");
    run_case("test3",
             "abc",
             "d",
             "x",
             true,
             "abc");
    run_case("test4",
             "abc",
             "",
             "x",
             true,
             "abc");
    return (0);
}
