/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_to_string_internal.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:16:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 16:18:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

/*
 * Return a string that is the concatenation of the elements in A from START
 * to END, separated by SEP.
 */
static char *array_to_string_internal (t_arr_elem *start,
										t_arr_elem *end,
										char *sep,
										int quoted)
{
	char		*result;
	char		*t;
	t_arr_elem	*ae;
	int			slen;
	int			rsize;
	int			rlen;
	int			reg;

	if (start == end)
		return ((char *)NULL);
	slen = ft_strlen(sep);
	result = NULL;
	for (rsize = rlen = 0, ae = start; ae != end; ae = element_forw(ae))
	{
		if (rsize == 0)
		{
			rsize = 64;
			result = (char *)malloc(rsize);
			if (!result)
				return ((char *)NULL);
		}
		if (element_value(ae))
		{
			t = quoted;
			if (t)
				quote_string(element_value(ae));
			else
				element_value(ae);
			reg = ft_strlen(t);
			resize_malloced_buffer(result, rlen, (reg + slen + 2), rsize, rsize);
			ft_strcpy(result + rlen, t);
			rlen += reg;
			if (quoted)
				free(t);
			if (element_forw(ae) != end)
			{
				ft_strcpy(result + rlen, sep);
				rlen += slen;
			}
		}
	}
	if (result)
	  result[rlen] = '\0';
	return(result);
}