/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_version.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 12:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/10/19 12:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_version.h"

/*
** The version this library was BUILT as. LIBFT_VERSION in the header is the
** version a consumer was COMPILED against; the two differ exactly when a
** program picks up a libft.so, or links a libft.a, from another release.
*/
const char	*ft_version(void)
{
	return (LIBFT_VERSION);
}
