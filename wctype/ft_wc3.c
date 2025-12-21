/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wblank.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:34:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 23:08:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wctype.h"

int	ft_iswblank(wint_t wc)
{
	return (wc == L' ' || wc == L'\t');
}

int	ft_iswcntrl(wint_t wc)
{
	return (wc <= 0x1F || wc == 0x7F);
}

int	ft_iswgraph(wint_t wc)
{
	return (wc > L' ' && wc <= L'~');
}

int	ft_iswpunct(wint_t wc)
{
	if (ft_iswalpha(wc) || ft_iswdigit(wc))
		return (0);
	if (wc >= L'!' && wc <= L'~')
		return (1);
	return (0);
}
