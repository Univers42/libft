#include "array.h"

/*
 * Return a string whose elements are the members of array A beginning at
 * index START and spanning NELEM members.  Null elements are counted.
 * Since arrays are sparse, unset array elements are not counted.
 */
// char *
// array_subrange (t_arr *a, arrayind_t start, arrayind_t nelem, int starsub, int quoted, int pflags)
// {
// 	t_arr		*a2;
// 	t_arr_elem	*h, *p;
// 	arrayind_t	i;
// 	char		*t;
// 	WORD_LIST	*wl;

// 	p = a ? array_head (a) : 0;
// 	if (p == 0 || array_empty (a) || start > array_max_index(a))
// 		return ((char *)NULL);

// 	/*
// 	 * Find element with index START.  If START corresponds to an unset
// 	 * element (arrays can be sparse), use the first element whose index
// 	 * is >= START.  If START is < 0, we count START indices back from
// 	 * the end of A (not elements, even with sparse arrays -- START is an
// 	 * index).
// 	 */
// 	for (p = element_forw(p); p != array_head(a) && start > element_index(p); p = element_forw(p))
// 		;

// 	if (p == a->head)
// 		return ((char *)NULL);

// 	/* Starting at P, take NELEM elements, inclusive. */
// 	for (i = 0, h = p; p != a->head && i < nelem; i++, p = element_forw(p))
// 		;

// 	a2 = array_slice(a, h, p);

// 	wl = array_to_word_list(a2);
// 	array_dispose(a2);
// 	if (wl == 0)
// 		return (char *)NULL;
// 	t = string_list_pos_params(starsub ? '*' : '@', wl, quoted, pflags);	/* XXX */
// 	dispose_words(wl);

// 	return t;
// }
