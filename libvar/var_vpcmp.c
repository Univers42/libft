/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_vpcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:11:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:42:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

int	var_vpcmp(const void *a, const void *b)
{
	return (libvar_varcmp(*(const char **)a, *(const char **)b));
}
