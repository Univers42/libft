/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wtoupper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:47:30 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 23:08:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wctype.h"

int	ft_towupper(wint_t wc)
{
	if (ft_iswlower(wc))
		return (wc - 32);
	return (wc);
}

int	ft_towlower(wint_t wc)
{
	if (ft_iswupper(wc))
		return (wc + 32);
	return (wc);
}

int	ft_iswupper(wint_t wc)
{
	return (wc >= L'A' && wc <= L'Z');
}

int	ft_iswxdigit(wint_t wc)
{
	return ((wc >= L'0' && wc <= L'9')
		|| (wc >= L'A' && wc <= L'F')
		|| (wc >= L'a' && wc <= L'f'));
}
