/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_walnum.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:34:52 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 23:09:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wctype.h"

int	ft_iswalnum(wint_t wc)
{
	return (ft_iswalpha(wc) || ft_iswdigit(wc));
}

int	ft_iswalpha(wint_t wc)
{
	return ((wc >= L'A' && wc <= L'Z') || (wc >= L'a' && wc <= L'z'));
}

int	ft_iswdigit(wint_t wc)
{
	return (wc >= L'0' && wc <= L'9');
}

/**
 * lightweight implementation . if we want full implementation
 * we would need data files or compressed tables.
 */
int	ft_wcwidth(wchar_t wc)
{
	if (wc >= 0x20 && wc <= 0x7E)
		return (1);
	if (wc < 0x20 || (wc >= 0x7F && wc <= 0x9F))
		return (-1);
	if ((wc >= 0x0300 && wc <= 0x036F)
		|| (wc >= 0x1AB0 && wc <= 0x1AFF))
		return (0);
	if ((wc >= 0x1100 && wc <= 0x115F)
		|| (wc >= 0x2E80 && wc <= 0xA4CF)
		|| (wc >= 0xAC00 && wc <= 0xD7A3)
		|| (wc >= 0xFF01 && wc <= 0xFF60)
		|| (wc >= 0xFFE0 && wc <= 0xFFE6))
		return (2);
	return (1);
}
