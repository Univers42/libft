/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atomax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:05:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:10:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include <stdlib.h>

intmax_t	ft_atomax(const char *s, int base)
{
	char	*endptr;

	return (strtoimax(s, &endptr, base));
}
