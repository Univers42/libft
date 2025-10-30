#include "array.h"

// t_arr *array_slice_safe(t_arr *array, size_t start, size_t end)
// {
// 	t_arr_elem *s, *e;
// 	size_t i;

// 	if (!array || array_empty(array) || start >= array_num_elements(array))
// 		return array_create();

// 	s = element_forw(array->head);
// 	for (i = 0; i < start && s != array->head; ++i)
// 		s = element_forw(s);
// 	if (s == array->head)
// 		return array_create();

// 	e = s;
// 	for (i = start; i < end && e != array->head; ++i)
// 		e = element_forw(e);

// 	return array_slice(array, s, e);
// }
