/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 03:39:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 03:53:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "../pipe.h"
#include "lifoba.h"

#ifdef __cplusplus
}
#endif

/* A more thorough test:
 * - allocate several buffers via lifoba (small/medium/large)
 * - child writes them to pipe (including several lines)
 * - parent reads raw bytes with read_all and verifies content
 * - parent reads lines via read_line_arena and prints them
 * - test set_nonblock / set_cloexec on fds
 */

int fill_pattern(char *buf, size_t n, char ch)
{
    for (size_t i = 0; i < n; ++i)
        buf[i] = (char)(ch + (i & 0x3));
    return 0;
}

int main(void)
{
    t_stack_mark mark;
    t_pipe_fds *p;
    pid_t pid;

    const size_t small_sz = 64;
    const size_t medium_sz = 64 * 1024;  /* 64KB */
    const size_t large_sz = 1024 * 1024; /* 1MB */

    set_stack_mark(&mark);

    p = pipe_create();
    if (!p)
    {
        perror("pipe_create");
        pop_stack_mark(&mark);
        return 1;
    }

    /* try set flags and report */
    if (set_cloexec(p->r, 1) == 0 && set_cloexec(p->w, 1) == 0)
        printf("[info] FD_CLOEXEC set on both ends\n");
    else
        printf("[warn] couldn't set FD_CLOEXEC\n");

    /* we flip nonblock on and off to verify calls succeed */
    if (set_nonblock(p->r, 1) == 0)
    {
        printf("[info] set_nonblock enabled on read end\n");
        set_nonblock(p->r, 0);
    }
    else
        printf("[warn] set_nonblock failed on read end\n");

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        pipe_close_pair(p);
        pop_stack_mark(&mark);
        return 1;
    }

    if (pid == 0)
    {
        /* child: close read end, write multiple buffers and lines, then exit */
        pipe_close_end(p, 0);

        /* allocate in arena (child may use lifoba too) */
        char *s_small = (char *)st_alloc(small_sz);
        char *s_med = (char *)st_alloc(medium_sz);
        char *s_large = (char *)st_alloc(large_sz);

        if (!s_small || !s_med || !s_large)
        {
            perror("st_alloc");
            pipe_close_pair(p);
            /* ensure arena mark is popped before exiting */
            pop_stack_mark(&mark);
            _exit(2);
        }

        fill_pattern(s_small, small_sz, 'A');
        fill_pattern(s_med, medium_sz, 'M');
        fill_pattern(s_large, large_sz, 'L');

        /* write small, medium, large sequentially */
        if (write_all(p->w, s_small, small_sz) < 0)
            perror("write_all small");
        if (write_all(p->w, s_med, medium_sz) < 0)
            perror("write_all medium");
        if (write_all(p->w, s_large, large_sz) < 0)
            perror("write_all large");

        /* send a few textual lines for read_line_arena test */
        const char *lines = "first line from child\nsecond line from child\n";
        if (write_all(p->w, lines, strlen(lines)) < 0)
            perror("write_all lines");

        pipe_close_pair(p);
        /* free arena allocations in child before exiting */
        pop_stack_mark(&mark);
        _exit(0);
    }

    /* parent */
    pipe_close_end(p, 1);

    /* read back the raw bytes: small + medium + large */
    size_t total = small_sz + medium_sz + large_sz;
    char *recv = (char *)malloc(total);
    if (!recv)
    {
        perror("malloc");
        wait(NULL);
        pipe_close_pair(p);
        pop_stack_mark(&mark);
        return 1;
    }
    ssize_t got = read_all(p->r, recv, total);
    if (got < 0)
    {
        perror("read_all");
    }
    else
    {
        printf("[info] read_all expected %zu bytes, got %zd bytes\n", total, got);
        /* verify patterns */
        int ok = 1;
        for (size_t i = 0; i < small_sz; ++i)
            if (recv[i] != (char)('A' + (i & 0x3)))
            {
                ok = 0;
                break;
            }
        for (size_t i = 0; ok && i < medium_sz; ++i)
            if (recv[small_sz + i] != (char)('M' + (i & 0x3)))
            {
                ok = 0;
                break;
            }
        for (size_t i = 0; ok && i < large_sz; ++i)
            if (recv[small_sz + medium_sz + i] != (char)('L' + (i & 0x3)))
            {
                ok = 0;
                break;
            }
        printf("[info] data verification: %s\n", ok ? "PASS" : "FAIL");
    }

    /* Now read textual lines that child wrote via read_line_arena.
     * read_line_arena returns strings allocated/committed in arena.
     * Ensure we still have the arena mark set while using them.
     */
    char *line;
    int count = 0;
    while ((line = read_line_arena(p->r)) != NULL && count < 10)
    {
        printf("[line %d] %s", ++count, line);
    }

    wait(NULL);
    free(recv);
    pipe_close_pair(p);
    pop_stack_mark(&mark);
    printf("[done] demo finished\n");
    return 0;
}