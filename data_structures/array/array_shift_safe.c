#include "array.h"

// t_arr_elem **array_shift_safe(t_arr *a, int n, int flags)
// {
// 	t_arr_elem **ret;
// 	int i;

// 	if (a == 0 || array_empty(a) || n <= 0)
// 		return ((t_arr_elem **)NULL);
// 	ret = (t_arr_elem **)malloc((n + 1) * sizeof(t_arr_elem *));
// 	if (!ret)
// 		return NULL;
// 	for (i = 0; i < n; ++i)
// 	{
// 		ret[i] = array_shift(a, 1, flags);
// 		if (!ret[i])
// 			break;
// 	}
// 	ret[i] = NULL;
// 	return ret;
// }
