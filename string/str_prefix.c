/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_prefix.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:08:00 by alcacere          #+#    #+#             */
/*   Updated: 2025/11/04 19:08:14 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*str_prefix(const char *string, const char *pfx)
{
	while (*pfx)
		if (*pfx++ != *string++)
			return (0);
	return ((char *) string);
}
