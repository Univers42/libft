#include "ft_memory.h"
#include <stdlib.h>
#include <string.h>

void *ft_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        // Handle allocation failure (e.g., log error, exit, etc.)
        return NULL;
    }
    return ptr;
}

void *ft_calloc(size_t count, size_t size)
{
    void *ptr = calloc(count, size);
    if (!ptr)
    {
        // Handle allocation failure (e.g., log error, exit, etc.)
        return NULL;
    }
    return ptr;
}

void *ft_realloc(void *ptr, size_t new_size)
{
    void *new_ptr = realloc(ptr, new_size);
    if (!new_ptr)
    {
        // Handle allocation failure (e.g., log error, exit, etc.)
        return NULL;
    }
    return new_ptr;
}

void ft_free(void *ptr)
{
    if (ptr)
    {
        free(ptr);
    }
}