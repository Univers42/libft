# ft_vector_create

Allocates and initializes an empty vector.

## Synopsis

```c
t_vector *ft_vector_create(void);
```

## Description

Creates an empty vector with an initial capacity. The vector stores `void*` elements and grows as needed.

## Return Value

A pointer to an allocated `t_vector` or `NULL` if allocation fails.
