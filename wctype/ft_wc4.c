/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wprint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:35:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 23:08:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wctype.h"

int	ft_iswprint(wint_t wc)
{
	return (wc >= L' ' && wc <= L'~');
}

int	ft_iswlower(wint_t wc)
{
	return (wc >= L'a' && wc <= L'z');
}

int	ft_iswspace(wint_t wc)
{
	return (wc == L' ' || wc == L'\t' || wc == L'\n'
		|| wc == L'\r' || wc == L'\v' || wc == L'\f');
}
