#ifndef FT_MEMORY_H
#define FT_MEMORY_H

#include <stdlib.h>

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_calloc(size_t count, size_t size);
void *ft_realloc(void *ptr, size_t size);

#endif // FT_MEMORY_H
