/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 02:12:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/06 18:51:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "readline.h"
#include "ft_vector.h"
#include "ft_string.h"
#include "ft_memory.h"
#include <fcntl.h>
#include <unistd.h>

static t_hist *get_history_state(void)
{
	static t_hist hist = {0};
	return (&hist);
}

static bool str_slice_eq_str(const char *slice, size_t slice_len, const char *str)
{
	size_t str_len = ft_strlen(str);
	if (slice_len != str_len)
		return (false);
	return (ft_strncmp(slice, str, slice_len) == 0);
}

static void add_history(const char *line)
{
	t_hist *h = get_history_state();
	char *dup = ft_strdup(line);
	if (dup)
		vec_push(&h->cmds, &dup);
}

static bool worthy_of_being_remembered(t_rl *rl, t_hist *h)
{
	if (rl->cursor > 1 && h->active && h->cmds.len > 0)
	{
		char *last_entry = *(char **)vec_get(&h->cmds, h->cmds.len - 1);
		if (!str_slice_eq_str(rl->str->buff, rl->cursor - 1, last_entry))
			return (true);
	}
	return (false);
}

void manage_history(t_rl *rl)
{
	t_hist *h = get_history_state();
	char *hist_entry;
	char *enc_hist_entry;

	if (worthy_of_being_remembered(rl, h))
	{
		hist_entry = ft_strndup(rl->str->buff, rl->cursor - 1);
		add_history(hist_entry);
		vec_push(&h->cmds, &hist_entry);
		if (h->append_fd >= 0)
		{
			t_dyn_str encoded = encode_cmd_hist(hist_entry);
			enc_hist_entry = encoded.buff;
			write(h->append_fd, enc_hist_entry, ft_strlen(enc_hist_entry));
			free(enc_hist_entry);
		}
		free(hist_entry);
	}
	rlreset(rl);
}

void init_history(void)
{
	t_hist *h = get_history_state();
	t_vec_config cfg = {
		.elem_size = sizeof(char *),
		.initial_capacity = 64,
		.type_mask = VEC_TYPE_PTR};

	*h = (t_hist){.append_fd = -1, .active = true};
	vec_init(&h->cmds, &cfg);
	dyn_str_init(&h->str);
}

void free_hist(void)
{
	t_hist *h = get_history_state();
	size_t i = 0;

	while (i < h->cmds.len)
	{
		char *entry = *(char **)vec_get(&h->cmds, i);
		free(entry);
		i++;
	}
	vec_destroy(&h->cmds);
	free(h->str.buff);
}

t_dyn_str parse_single_cmd(t_dyn_str hist, size_t *cur)
{
	t_dyn_str ret;
	bool bs = false;
	char c;

	dyn_str_init(&ret);
	while (*cur < hist.len)
	{
		c = hist.buff[*cur];
		if (c == '\\' && !bs)
			bs = true;
		else if (c == '\n' && !bs)
		{
			(*cur)++;
			break;
		}
		else
		{
			dyn_str_push(&ret, c);
			bs = false;
		}
		(*cur)++;
	}
	return (ret);
}

t_vec parse_hist_file(t_dyn_str *str)
{
	t_vec ret;
	t_vec_config cfg = {.elem_size = sizeof(char *), .type_mask = VEC_TYPE_PTR};
	size_t cur = 0;

	vec_init(&ret, &cfg);
	while (cur < str->len)
	{
		t_dyn_str cmd_str = parse_single_cmd(*str, &cur);
		char *cmd = cmd_str.buff;
		vec_push(&ret, &cmd);
	}
	return (ret);
}

char *get_hist_file_path(t_vec *ldenv)
{
	const char *home = getenv("HOME");
	t_dyn_str abspath;

	(void)ldenv; /* suppress unused parameter warning */
	if (!home)
		return (NULL);

	dyn_str_init(&abspath);
	dyn_str_pushstr(&abspath, (char *)home); /* cast away const */
	dyn_str_pushstr(&abspath, "/.hellish_history");

	return (abspath.buff);
}

void parse_history_file(t_hist *ldhist, t_vec *ldenv)
{
	char *phistfile;
	int fd;

	phistfile = get_hist_file_path(ldenv);
	if (!phistfile)
		return;

	fd = open(phistfile, O_RDONLY);
	if (fd < 0)
	{
		free(phistfile);
		return;
	}

	dyn_str_append_fd(fd, &ldhist->str);
	close(fd);

	ldhist->cmds = parse_hist_file(&ldhist->str);
	ldhist->append_fd = open(phistfile, O_CREAT | O_WRONLY | O_APPEND, 0666);
	free(phistfile);
}

t_dyn_str encode_cmd_hist(char *cmd)
{
	t_dyn_str ret;
	size_t i = 0;

	dyn_str_init(&ret);
	while (cmd[i])
	{
		if (cmd[i] == '\n')
			dyn_str_pushstr(&ret, "\\\n");
		else
			dyn_str_push(&ret, cmd[i]);
		i++;
	}
	dyn_str_push(&ret, '\n');
	return (ret);
}

void rl_history_init(void)
{
	init_history();
}

void rl_history_add(const char *line)
{
	add_history(line);
}

char *rl_history_prev(void)
{
	t_hist *h = get_history_state();
	char *result;

	if (h->cmds.len == 0 || h->iter_idx >= h->cmds.len)
		return (NULL);
	result = *(char **)vec_get(&h->cmds, h->iter_idx);
	h->iter_idx++;
	return (result);
}

char *rl_history_next(void)
{
	t_hist *h = get_history_state();

	if (h->iter_idx == 0)
		return (NULL);
	h->iter_idx--;
	if (h->iter_idx >= h->cmds.len)
		return (NULL);
	return (*(char **)vec_get(&h->cmds, h->iter_idx));
}

void rl_history_reset_index(void)
{
	t_hist *h = get_history_state();
	h->iter_idx = 0;
}

void rl_history_free(void)
{
	free_hist();
}

int rl_history_find_prev(const char *pat, int from_index)
{
	(void)pat;
	(void)from_index;
	return (-1);
}

const char *rl_history_get(int idx)
{
	t_hist *h = get_history_state();
	if (idx < 0 || (size_t)idx >= h->cmds.len)
		return (NULL);
	return (*(char **)vec_get(&h->cmds, idx));
}