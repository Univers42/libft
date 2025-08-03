/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_translate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:38:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/03 15:55:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrices.h"

/*
** Sets the matrix to a translation transformation by (tx, ty, tz).
** Usage: matrix_translate(&mat, tx, ty, tz);
** Expects: Pointer to a valid t_mat4.
*/

void    matrix_translate(t_mat4 *out, float tx, float ty, float tz)
{
    matrix_identity(out);
    out->m[0][3] = tx;
    out->m[1][3] = ty;
    out->m[2][3] = tz;
}
