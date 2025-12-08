/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 02:12:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 02:59:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_readline.h"
#include "ft_vector.h"
#include "ft_string.h"
#include "ft_memory.h"
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "trap.h"
#include "lexer.h"
#include "var.h"
#include <errno.h>
#include <string.h>

void buff_readline_reset(t_rl *rl)
{
	ft_memmove(rl->str.buff, rl->str.buff + rl->cursor, rl->str.len - rl->cursor);
	rl->str.len -= rl->cursor;
	if (rl->str.buff)
		rl->str.buff[rl->str.len] = 0;
	rl->cursor = 0;
	buff_readline_update(rl);
}


void print_history(t_hist *hist)
{
	size_t i;
	char *cmd;

	i = 0;
	printf("\n%s=== HISTORY CONTENTS === %s\n", CYAN_TERM, RESET_TERM);
	while (i < hist->cmds.len)
	{
		cmd = *((char **)hist->cmds.buff + i);
		printf("%s[%zu]%s %s%s%s\n", BLACK_TERM, i, RESET_TERM, GREEN_TERM, cmd, RESET_TERM);
		i++;
	}
	printf("%s==========================\n%s", CYAN_TERM, RESET_TERM);
}











void set_cmd_status(t_status res, t_status *last_cmd_status_res, char *last_cmd_status_s)
{
	*last_cmd_status_res = res;
	free(last_cmd_status_s);
	last_cmd_status_s = ft_itoa(res.status);
}


t_dyn_str parse_single_cmd(t_dyn_str hist, size_t *cur)
{
    t_dyn_str cmd;
    bool esc;

    dyn_str_init(&cmd);
    esc = false;
    while (*cur < hist.len)
    {
        char c = hist.buff[*cur];
        if (!esc && c == ESC_BS)
        {
            esc = true;
            (*cur)++;
            continue;
        }
        if (!esc && c == '\n')
        {
            /* end-of-encoded-command marker */
            (*cur)++;
            break;
        }
        if (esc)
        {
            /* decode escape sequences you encoded above */
            if (c == 'n')
                dyn_str_push(&cmd, '\n');
            else
                dyn_str_push(&cmd, c); /* backslash or any escaped char */
            esc = false;
            (*cur)++;
            continue;
        }
        dyn_str_push(&cmd, c);
        (*cur)++;
    }
    return cmd;
}

/**
 * !? Set connfig struct , do not forget
 */
t_vec parse_hist_file(t_dyn_str hist)
{
	size_t cur;
	t_vec ret;
	char *cmd;
	t_dyn_str cmd_str;
	t_vec_config config;

	config = (t_vec_config){.elem_size = sizeof(char *), .initial_capacity = 32, .type_mask = VEC_TYPE_PTR};

	cur = 0;
	if (vec_init(&ret, &config) < 0)
		return (ret);
	while (cur < hist.len)
	{
		cmd_str = parse_single_cmd(hist, &cur);
		cmd = cmd_str.buff; /* Keep the allocated buffer */
		vec_push(&ret, &cmd);
		add_history(cmd);
		/* Don't free cmd_str.buff here - it's now owned by the vector */
	}
	return (ret);
}

static void hexdump_debug(const char *buf, size_t len, size_t max_print)
{
    size_t j;

    if (!buf)
    {
        printf("  <NULL buf>\n");
        return;
    }
    if (len == 0)
    {
        printf("  <empty buf>\n");
        return;
    }
    printf("  hexdump (len=%zu):\n", len);
    for (j = 0; j < len && j < max_print; ++j)
        printf("   [%02zu] '%c' 0x%02x\n",
            j,
            (buf[j] >= 32 && buf[j] < 127) ? buf[j] : '.',
            (unsigned char)buf[j]);
    if (j < len)
        printf("   ... (%zu more bytes)\n", len - j);
}

char *get_hist_file_path(t_vec *ldenv)
{
    t_env *env;
    t_dyn_str full_path;
    char *result;
    const char *home = NULL;
    char *g_home;
    size_t hlen;

    env = env_get(ldenv, "HOME");
    printf("DBG: env_get(ldenv, \"HOME\") = %p\n", (void*)env);
    if (env)
        printf("DBG: env->value = %p; value='%s'\n", (void*)env->value, env->value ? env->value : "(null)");
    g_home = getenv("HOME");
    printf("DBG: getenv(\"HOME\") = %p; value='%s'\n", (void*)g_home, g_home ? g_home : "(null)");

    if (env && env->value && env->value[0] != '\0')
        home = env->value;
    else
        home = g_home;

    printf("DBG: chosen home = %p; value='%s'\n", (void*)home, home ? home : "(null)");
    if (!home || !*home)
    {
        warning_error("HOME is not set, can't get the history");
        return (0);
    }

    /* debug the source home bytes before copying */
    hlen = strlen(home);
    printf("DBG: home strlen=%zu\n", hlen);
    hexdump_debug(home, hlen, 128);

    dyn_str_init(&full_path);
    printf("DBG: full_path after init: buff=%p len=%zu\n", (void*)full_path.buff, full_path.len);
    dyn_str_pushstr(&full_path, home);
    printf("DBG: after pushstr(home): buff=%p len=%zu\n", (void*)full_path.buff, full_path.len);
    hexdump_debug(full_path.buff, full_path.len, 64);

    if (!dyn_str_ends_with_str(&full_path, "/"))
    {
        dyn_str_push(&full_path, '/');
        printf("DBG: after push('/') : buff=%p len=%zu\n", (void*)full_path.buff, full_path.len);
        hexdump_debug(full_path.buff, full_path.len, 64);
    }
    dyn_str_pushstr(&full_path, HIST_FILE);
    printf("DBG: after pushstr(HIST_FILE) : buff=%p len=%zu\n", (void*)full_path.buff, full_path.len);
    hexdump_debug(full_path.buff, full_path.len, 128);

    printf("DBG: HIST_FILE='%s'\n", HIST_FILE);
    printf("DBG: constructed history path (as string) = '%s'\n", full_path.buff ? full_path.buff : "(null)");

    result = ft_strdup(full_path.buff);
    free(full_path.buff);
    return (result);
}

// ...existing code...
/* small local free_fn for history vector */
static void free_hist_cmd(void *elem)
{
	char **p;

	if (!elem)
		return;
	p = (char **)elem;
	free(*p);
}

void parse_history_file(t_hist *h, t_vec *env)
{
    t_dyn_str	hist;
    int			fd;
    char		*hist_file_path;
    int			tmp;

    hist_file_path = get_hist_file_path(env);
    if (!hist_file_path)
        return ;

    /* Try open for read, create empty file if it does not exist. */
    fd = open(hist_file_path, O_RDONLY);
    if (fd < 0)
    {
        tmp = open(hist_file_path, O_CREAT | O_WRONLY, 0644);
        if (tmp >= 0)
            close(tmp);
        fd = open(hist_file_path, O_RDONLY);
    }
    if (fd < 0)
    {
        ft_eprintf("Can't open the history file for reading: %s (%s)\n",
            hist_file_path, strerror(errno));
        free(hist_file_path);
        h->append_fd = -1;
        return;
    }

    dyn_str_init(&hist);
    if (!dyn_str_append_fd(fd, &hist))
    {
        ft_eprintf("Failed to read history file: %s\n", hist_file_path);
        close(fd);
        free(hist_file_path);
        if (hist.buff)
            free(hist.buff);
        h->append_fd = -1;
        return;
    }
    close(fd);

    /* Parse into in-memory vector */
    h->cmds = parse_hist_file(hist);

    /* Open append fd for future writes */
    h->append_fd = open(hist_file_path, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (h->append_fd < 0)
        ft_eprintf("Can't open the history file for writing: %s (%s)\n",
            hist_file_path, strerror(errno));

    free(hist_file_path);
    if (hist.buff)
        free(hist.buff);
}

void init_history(t_hist *h, t_vec *env)
{
	t_vec_config config;

	config = (t_vec_config){
		.elem_size = sizeof(char *),
		.initial_capacity = 32,
		.type_mask = VEC_TYPE_PTR,
		.copy_fn = NULL,
		.free_fn = free_hist_cmd};
	vec_init(&h->cmds, &config);
	h->append_fd = -1;
	h->active = true;
	parse_history_file(h, env);
}

void free_hist(t_hist *h)
{
	vec_destroy(&h->cmds);
	if (h->append_fd >= 0)
		close(h->append_fd);
}

t_dyn_str encode_cmd_hist(const char *cmd)
{
    t_dyn_str out;
    size_t i;
    char c;

    dyn_str_init(&out);
    if (!cmd)
        return out;
    for (i = 0; cmd[i]; ++i)
    {
        c = cmd[i];
        if (c == '\\')          /* escape backslash */
        {
            dyn_str_push(&out, ESC_BS);
            dyn_str_push(&out, '\\');
        }
        else if (c == '\n')    /* escape newline */
        {
            dyn_str_push(&out, ESC_BS);
            dyn_str_push(&out, 'n'); /* or ESC_NL byte if you use a special byte */
        }
        else
            dyn_str_push(&out, c);
    }
    /* final terminator: a real newline that separates encoded commands on disk */
    dyn_str_push(&out, '\n');

    /* out.len is maintained by dyn_str_push; return structure */
    return out;
}



bool worthy_of_being_remembered(t_rl *rl, t_hist *h)
{
    const char *s;
    size_t i;
    size_t len;
    size_t first_non_ws = 0;

    (void)h;
    if (!rl || !rl->str.buff)
    {
        printf("DBG: worthy: no rl or empty buffer\n");
        return false;
    }

    s = rl->str.buff;
    /* Use the actual buffer length, cursor arithmetic is fragile for multiline/continuation */
    len = strlen(s);
    if (len == 0)
    {
        printf("DBG: worthy: len==0 -> false\n");
        return false;
    }

    /* find first non-whitespace in the entered range */
    for (i = 0; i < len; ++i)
    {
        if (!isspace((unsigned char)s[i]))
        {
            first_non_ws = i;
            break;
        }
    }
    if (i == len)
    {
        printf("DBG: worthy: only whitespace -> false (line='%.*s')\n", (int)len, s);
        return false;
    }

    printf("DBG: worthy: line='%.*s' (len=%zu), first_non_ws=%zu\n",
           (int)len, s, len, first_non_ws);

    /* Accept any non-empty, non-whitespace line for now */
    return true;
}

// ...existing code...
void manage_history(t_rl *rl, t_hist *h)
{
    char *hist_entry;
    t_dyn_str enc;
    ssize_t r;
    size_t written;
    size_t entry_len;

    printf("DBG: manage_history invoked\n");

    if (!worthy_of_being_remembered(rl, h))
    {
        printf("DBG: not worthy_of_being_remembered\n");
        buff_readline_reset(rl);
        return;
    }

    /* copy the whole readline buffer (no cursor arithmetic) */
    hist_entry = ft_strndup(rl->str.buff, strlen(rl->str.buff));
    if (!hist_entry)
    {
        printf("DBG: hist_entry allocation failed\n");
        buff_readline_reset(rl);
        return;
    }

    /* strip trailing newline/carriage-return if present so add_history stores a clean line */
    entry_len = strlen(hist_entry);
    while (entry_len > 0 && (hist_entry[entry_len - 1] == '\n' || hist_entry[entry_len - 1] == '\r'))
    {
        hist_entry[--entry_len] = '\0';
    }

    /* keep in-memory history (do NOT free hist_entry after this) */
    add_history(hist_entry);
    vec_push(&h->cmds, &hist_entry);

    printf("DBG: manage_history: append_fd=%d entry='%s'\n", h->append_fd, hist_entry);

    /* encode for disk (encoder appends its own terminator) */
    enc = encode_cmd_hist(hist_entry);
    printf("DBG: encode -> len=%zu buff=%p\n", enc.len, (void *)enc.buff);
    if (enc.buff && enc.len > 0)
        hexdump_debug(enc.buff, enc.len, enc.len > 128 ? 128 : enc.len);

    /* write encoded bytes using explicit length (handle partial writes/EINTR) */
    if (h->append_fd >= 0 && enc.buff && enc.len > 0)
    {
        written = 0;
        while (written < enc.len)
        {
            r = write(h->append_fd, enc.buff + written, enc.len - written);
            if (r < 0)
            {
                if (errno == EINTR)
                    continue;
                fprintf(stderr, "history: write failed fd=%d: %s\n", h->append_fd, strerror(errno));
                close(h->append_fd);
                h->append_fd = -1;
                break;
            }
            if (r == 0)
                break;
            written += (size_t)r;
        }
        if (h->append_fd >= 0)
            fsync(h->append_fd);
        printf("DBG: wrote %zu/%zu encoded bytes to history\n", written, enc.len);
    }
    else
        printf("DBG: skipping write (append_fd=%d enc.len=%zu)\n", h->append_fd, enc.len);

    if (enc.buff)
        free(enc.buff);

    /* DO NOT free(hist_entry) here — stored in h->cmds */
    buff_readline_reset(rl);
}