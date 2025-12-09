/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:06:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 18:24:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

extern "C"
{
#include "ft_stdlib.h"
#include "ft_memory.h"
#include "pool.h"
}

#define BUFFER_SIZE 1024
#define SMALL_BLOCK_SIZE 128
#define MEDIUM_BLOCK_SIZE 1024
#define LARGE_BLOCK_SIZE 4096

typedef struct s_fd_list
{
    int fd;
    char *memory;
    struct s_fd_list *next;
} t_fd_list;

static t_pool small_pool, medium_pool, large_pool;

void *ft_memmove(void *dst, const void *src, size_t n)
{
    unsigned char *d;
    const unsigned char *s;

    d = (unsigned char *)dst;
    s = (const unsigned char *)src;
    if (d == s || n == 0)
        return (dst);
    if (d < s)
        while (n--)
            *d++ = *s++;
    else
    {
        d += n;
        s += n;
        while (n--)
            *--d = *--s;
    }
    return (dst);
}

size_t ft_strclen(const char *s, int c)
{
    size_t i;

    i = 0;
    while (s && s[i] && s[i] != (char)c)
        i++;
    return (i);
}

char *ft_strndup_pool(const char *s, size_t n)
{
    char *dup;

    if (n <= 64)
        dup = (char *)pool_alloc(&small_pool);
    else if (n <= 256)
        dup = (char *)pool_alloc(&medium_pool);
    else
        dup = (char *)pool_alloc(&large_pool);
    if (!dup)
        return (NULL);
    ft_memmove(dup, s, n);
    dup[n] = '\0';
    return (dup);
}

char *ft_strjoin_pool(char *s1, const char *s2)
{
    char *new_str;
    char *ptr;
    size_t len1;
    size_t len2;

    len1 = ft_strclen(s1, '\0');
    len2 = ft_strclen(s2, '\0');
    if (len1 + len2 + 1 <= 64)
        new_str = (char *)pool_alloc(&small_pool);
    else if (len1 + len2 + 1 <= 256)
        new_str = (char *)pool_alloc(&medium_pool);
    else
        new_str = (char *)pool_alloc(&large_pool);
    if (!new_str)
        return (NULL);
    ptr = new_str;
    if (s1)
        while (*s1)
            *ptr++ = *s1++;
    while (*s2)
        *ptr++ = *s2++;
    *ptr = '\0';
    return (new_str);
}

t_fd_list *ft_get_fd_node_pool(t_fd_list **fd_list, int fd)
{
    t_fd_list *current;
    t_fd_list *new_node;

    current = *fd_list;
    while (current)
    {
        if (current->fd == fd)
            return (current);
        current = current->next;
    }
    new_node = (t_fd_list *)pool_alloc(&small_pool);
    if (!new_node)
        return (NULL);
    new_node->fd = fd;
    new_node->memory = NULL;
    new_node->next = *fd_list;
    *fd_list = new_node;
    return (new_node);
}

static char *ft_store_chunks(int fd, char *memory)
{
    char *buffer;
    ssize_t bytes_read;

    buffer = (char *)pool_alloc(&medium_pool);
    if (!buffer)
        return (NULL);
    while (ft_strclen(memory, '\n') == ft_strclen(memory, '\0'))
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read <= 0)
        {
            if (bytes_read == 0)
                return (memory);
            return (NULL);
        }
        buffer[bytes_read] = '\0';
        memory = ft_strjoin_pool(memory, buffer);
        if (!memory)
            return (NULL);
    }
    return (memory);
}

static char *ft_get_line(char *memory)
{
    size_t newline_idx;
    char *line;

    if (!memory || !*memory)
        return (NULL);
    newline_idx = ft_strclen(memory, '\n');
    line = (char *)pool_alloc(&small_pool);
    if (!line)
        return (NULL);
    ft_memmove(line, memory, newline_idx + 1);
    line[newline_idx + 1] = '\0';
    return (line);
}

static char *ft_get_leftover(char *memory)
{
    char *ptr;

    ptr = memory;
    while (*ptr && *ptr != '\n')
        ptr++;
    if (!*ptr)
        return (NULL);
    return (ft_strndup_pool(ptr + 1, ft_strclen(ptr + 1, '\0'))); // Use pool version
}

char *get_next_line_pool(int fd)
{
    static t_fd_list *fd_list = NULL;
    t_fd_list *fd_node;
    char *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;

    fd_node = ft_get_fd_node_pool(&fd_list, fd);
    if (!fd_node)
        return NULL;

    fd_node->memory = ft_store_chunks(fd, fd_node->memory);
    if (!fd_node->memory)
        return NULL;
    line = ft_get_line(fd_node->memory);
    fd_node->memory = ft_get_leftover(fd_node->memory);
    return (line);
}

int main(int argc, char **argv)
{
    clock_t start, end;
    double cpu_time_used;
    char *line;
    int fd, i = 0;

    if (argc < 2)
        return (fprintf(stderr, "Usage: %s <filename>\n", argv[0]), 1);

    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return (perror("open"), 1);

    // Initialize pools
    printf("=== Pool Allocator Test ===\n");
    pool_init(&small_pool, SMALL_BLOCK_SIZE, 100);
    pool_init(&medium_pool, MEDIUM_BLOCK_SIZE, 50);
    pool_init(&large_pool, LARGE_BLOCK_SIZE, 20);

    start = clock();

    while ((line = get_next_line_pool(fd)))
    {
        printf("Line %d: %s", ++i, line);
        // Note: In pool allocator, we don't free individual allocations
        // They are freed all at once when pools are destroyed
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n=== Pool Performance Results ===\n");
    printf("Lines processed: %d\n", i);
    printf("Time taken: %f seconds\n", cpu_time_used);

    pool_stats(&small_pool);
    pool_stats(&medium_pool);
    pool_stats(&large_pool);

    pool_destroy(&small_pool);
    pool_destroy(&medium_pool);
    pool_destroy(&large_pool);

    close(fd);
    return 0;
}