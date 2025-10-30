#include "array.h"

// void array_walk(t_arr *a, void *user, void (*func)(t_arr_elem *, void *))
// {
// 	t_arr_elem *cur;

// 	if (!a || array_empty(a) || !func)
// 		return;
// 	for (cur = element_forw(a->head); cur != a->head; cur = element_forw(cur))
// 		func(cur, user);
// }
