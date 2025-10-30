/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_to_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:50:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 21:04:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

// char	*array_to_string (t_arr *a, char *sep, int quoted)
// {
// 	if (a == 0)
// 		return((char *)NULL);
// 	if (array_empty(a))
// 		return(savestring(""));
// 	return (array_to_string_internal (element_forw(a->head), a->head, sep, quoted));
// }