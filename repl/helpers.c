/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:53:38 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/14 00:22:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void free_tab(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// 0 on success
int	write_to_file(char *str, int fd)
{
	int	wrote_total;
	int	wrote;
	int	len;

	len = ft_strlen(str);
	wrote_total = 0;
	while (wrote_total != len)
	{
		wrote = write(fd, str + wrote_total, len - wrote_total);
		if (wrote < 0)
			return (1);
		wrote_total += wrote;
	}
	return (0);
}

static void	default_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void forward_exit_status(t_status res)
{
	ft_assert(res.status != -1);
	if (res.c_c)
	{
		default_signal_handlers();
		kill(0, SIGINT);
		while (true)
			;
	}
	exit(res.status);
}

/**
 * We need the function to update teh caller's char* pointer (state->last_cmd_status_s).
 * in C alll arguments are passed by value, so passing char* gives teh function a copy of the
 * pointer. Freeing via that copy will free the same heap block but assigning a new
 * malloc/ft_itoa result tht local copy does not update the caller's pointer.
 * Teh new pointer is lost and leaks
 *
 * PINPOINT: change to char *** when teh function must change the caller's pointer itserlf otherwise
 * return new pointer and let the caller assign it
 */
void set_cmd_status(t_status *last_cmd_status_res, t_status res, char **last_cmd_status_s)
{
	*last_cmd_status_res = res;
	if (*last_cmd_status_s)
		free(*last_cmd_status_s);
	*last_cmd_status_s = ft_itoa(res.status);
}
