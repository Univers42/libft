/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:22:26 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/30 01:43:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "format.h"

// Helper to get log state string
static const char   *log_state_str(t_state state)
{
	if (state == ST_WARNING_BASE)
		return ("WARNING");
	else if (state == ST_INFO_BASE)
		return ("INFO");
	else if (state == ST_OK)
		return ("SUCCESS");
	else if (state == ST_ERR_BASE)
		return ("FAILURE");
	else
		return ("LOG");
}

// Enhanced log_print: accepts file for context
int log_print(t_log *state, const char *file, const char *format, ...)
{
	va_list ap;
	int     ret;

	ret = 0;
	if (!state || !file || !format || state->fd < 0)
		return (-1);
	if (state->state == ST_WARNING_BASE || state->state == ST_ERR_BASE)
		ret += ft_dprintf(state->fd, "[%s] %s -> ",
					log_state_str(state->state), file);
	else if (state->state == ST_INFO_BASE || state->state == ST_OK)
		ret += ft_dprintf(state->fd, "[%s] ", log_state_str(state->state));
	else
		ret += ft_dprintf(state->fd, "[LOG] ");
	va_start(ap, format);
	ret += ft_vdprintf(state->fd, format, &ap);
	va_end(ap);
	ret += ft_dprintf(state->fd, "\n");
	return (ret);
}
