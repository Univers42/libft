/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_opt_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:57:05 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:06:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

/*
 * Minimal placeholder for restoring option-like local variables.
 * The real implementation should restore shell option state from the
 * saved localvar record. Keep this as a no-op to allow the build to proceed.
 */

void restore_opt_var(t_localvar *lvp, t_var_state *state)
{
	(void)lvp;
	(void)state;
	/* no-op placeholder */
}
