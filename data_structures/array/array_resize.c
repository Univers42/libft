#include "array.h"

// void array_resize(t_arr *a, size_t newsize)
// {
// 	if (!a)
// 		return;
// 	if (newsize == 0)
// 	{
// 		array_flush(a);
// 		return;
// 	}
// 	/* For now, just ensure elements array is large enough, but since we're using linked list, maybe no-op */
// 	if (a->elements && a->alloc_size < newsize)
// 	{
// 		a->elements = realloc(a->elements, newsize * sizeof(t_arr_elem *));
// 		if (a->elements)
// 			a->alloc_size = newsize;
// 	}
// }
