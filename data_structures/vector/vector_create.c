#include "ft_vector.h"

t_vector    *ft_vector_create(void)
{
	t_vector	*vector;

	vector = malloc(sizeof(t_vector));
	if (!vector)
		return (NULL);
	vector->data = malloc(sizeof(void *) * VECTOR_INITIAL_CAPACITY);
	if (!vector->data)
		return (free(vector), NULL);
	vector->size = 0;
	vector->capacity = VECTOR_INITIAL_CAPACITY;
	return (vector);
}