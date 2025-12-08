/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 00:16:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 01:33:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>

extern "C"
{
	#include "libft.h"
	#include "ft_readline.h"
}

//static const char *CLR_RESET = "\033[0m";
//static const char *CLR_BOLD = "\033[1m";
//static const char *CLR_BLUE = "\033[34m";
//static const char *CLR_CYAN = "\033[36m";
//static const char *CLR_MAGENTA = "\033[35m";
//static const char *CLR_GREEN = GREEN_TERM;
typedef struct s_app
{
    t_dyn_str   input;
    t_dyn_str	cwd;
    t_vec		env;
    t_hist		hist;
    int         input_method;
    char        *base_context;
    char        *context;
    char		*pid;
    char        *last_cmd_status_s;
    t_status	last_cmd_status_res;
    bool		should_exit;
    t_rl		rl;
}	t_app;


typedef enum e_res_t
{
    RES_OK,
    RES_FATALERROR,
    RES_MOREINPUT,
    RES_INIT,
}t_res_t;
typedef struct s_pars
{
    t_vec	parse_stack;
    t_res_t	res;
}	t_pars;


void	init_cwd(t_dyn_str *_cwd)
{
    char	*cwd;

    dyn_str_init(_cwd);
    cwd = getcwd(NULL, 0);
    if (cwd)
        dyn_str_pushstr(_cwd, cwd);
    else
    {
        ft_eprintf("shell-init: error retrieving current directory\n"
            " getcwd: cannot access parent directories:"
            " No such file or directory\nsh: 0:"
            " getcwd() failed: No such file or directory\n");
    }
    free(cwd);
}

t_status	res_status(int status)
{
    t_status s = {};
    s.status = status;
    s.pid = -1;
    s.c_c = 0;
    return (s);
}

void	init_setup(t_app *shell, char **argv, char **envp)
{
    t_vec_config	env_config;

    set_unwind_sig();
    memset(shell, 0, sizeof *shell);
    shell->pid = xgetpid();
    shell->context = ft_strdup(argv[0]);
    shell->base_context = ft_strdup(argv[0]);
    set_cmd_status(res_status(0), &shell->last_cmd_status_res, shell->last_cmd_status_s);
    shell->last_cmd_status_res = res_status(0);
    init_cwd(&shell->cwd);
    env_config = (t_vec_config){
        .elem_size = sizeof(t_env),
        .initial_capacity = 32,
        .type_mask = VEC_TYPE_CUSTOM,
        .copy_fn = NULL,
        .free_fn = NULL
    };
    vec_init(&shell->env, &env_config);
    shell->env = env_to_vec_env(&shell->cwd, envp);
    init_history(&shell->hist, &shell->env);
    
}

void	cleanup_app(t_app *shell)
{
    free(shell->cwd.buff);
    free(shell->rl.str.buff);
    free_hist(&shell->hist);
    vec_destroy(&shell->env);
	free(shell->pid);
}


void	default_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	forward_exit_status(t_status res)
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

void	free_all_state(t_app *shell)
{
    free(shell->input.buff);
    shell->input = (t_dyn_str){};
    free(shell->last_cmd_status_s);
    /* pid is not heap allocated here */
    free(shell->context);
    free(shell->base_context);
	free(shell->pid);
    shell->context = 0;
    shell->base_context = 0;
    free(shell->rl.str.buff);
    free_hist(&shell->hist);
    free(shell->cwd.buff);
}

int	readline_cmd(t_app *shell, char **prompt)
{
    int	stat;

    stat = buff_readline(&shell->rl, &shell->input, *prompt, shell->input_method, shell->last_cmd_status_s, &shell->last_cmd_status_res, shell->context, shell->base_context);
    free(*prompt);
    *prompt = 0;
    if (stat == 0)
        return (1);
    if (stat == 2)
    {
        if (shell->input_method != INP_READLINE)
            shell->should_exit = true;
        return (2);
    }
    return (0);
}


bool	ends_with_bs_nl(t_dyn_str s)
{
	size_t	i;
	bool	unterminated;

	if (s.len == 0)
		return (false);
	i = s.len;
	unterminated = false;
	if (s.buff[--i] != '\n')
		return (false);
	while (i > 0)
	{
		i--;
		if (s.buff[i] == '\\')
			unterminated = !unterminated;
		else
			break ;
	}
	return (unterminated);
}

void	extend_bs(t_app *shell)
{
    char	*prompt;

    while (ends_with_bs_nl(shell->input))
    {
        dyn_str_pop(&shell->input);
        dyn_str_pop(&shell->input);
        prompt = ft_strdup("> ");
        if (readline_cmd(shell, &prompt))
            return ;
    }
}

static void get_more_tokens(t_app *shell, char **prompt, t_deque *tt)
{
    int	stat;

    while (*prompt)
    {
        stat = readline_cmd(shell, prompt);
        if (stat == 1)
        {
            if (tt->ctx && *(char *)tt->ctx && shell->input.len)
                ft_eprintf("%s: unexpected EOF while looking for "
                    "matching '%c'\n", shell->context, *(char *)tt->ctx);
            else if (shell->input.len)
                    ft_eprintf("%s: syntax error: unexpected enf of file\n",
                        shell->context);
            if (shell->input_method == INP_READLINE)
                    ft_eprintf("exit\n");
            if (!shell->last_cmd_status_res.status && shell->input.len)
            {
                t_status s = {};
                s.status = SYNTAX_ERR;
                s.pid = -1;
                s.c_c = 0;
                set_cmd_status(s, &shell->last_cmd_status_res, shell->last_cmd_status_s);
            }
            shell->should_exit = true;
        }
        if (stat)
            return ;
        *prompt = (extend_bs(shell), tokenizer(shell->input.buff, tt));
        if (*prompt)
            *prompt = ft_strdup(*prompt);
    }
}


static bool	try_parse_tokens(t_app *shell, t_pars *parser, t_deque *tt)
{
    if (is_empty_token_list(tt))
    {
        buff_readline_reset(&shell->rl);
        return (false);
    }
    parser->parse_stack = {};
    parser->res = RES_OK;
    return (true);
}

static void get_more_input_parser(t_app *shell, t_pars *parser, char **prompt, t_deque *tt)
{
    while (parser->res == RES_MOREINPUT || parser->res == RES_INIT)
    {
        get_more_tokens(shell, prompt, tt);
        if (!try_parse_tokens(shell, parser, tt))
            break ;
    }
}

void	parse_and_execute_input(t_app *shell)
{
    t_deque	tt;
    char	*prompt;
    t_pars	parser;
    char	*looking_for;

    looking_for = (char*)xmalloc(1);
    if (!looking_for)
        return;
    *looking_for = 0;
    parser = {};
    parser.res = RES_INIT;
    prompt = prompt_normal(shell->last_cmd_status_s, &shell->last_cmd_status_res).buff;
    if (deque_init(&tt, 64, sizeof(t_token), looking_for) != 0)
    {
        free(looking_for);
        warning_error((char*)"Couldn't initialize correct the token type inside the"
            "struct of deque");
        return ;
    }
    ft_memset(tt.buf, 0, tt.cap * tt.elem_size);
    get_more_input_parser(shell, &parser, &prompt, &tt);
    if (get_g_sig()->should_unwind)
    {
        t_status s = {};
        s.status = CANCELED;
        s.pid = -1;
        s.c_c = true;
        set_cmd_status(s, &shell->last_cmd_status_res, shell->last_cmd_status_s);
    }
    free(parser.parse_stack.buff);
    parser.parse_stack = {};
    free(tt.buf);
    free(tt.ctx);
    tt.ctx = NULL;
    tt.buf = NULL;
    shell->should_exit |= (get_g_sig()->should_unwind && shell->input_method != INP_READLINE) || shell->rl.has_finished;
}

int	main(int argc, char **argv, char **envp)
{
    t_app	shell;

    (void)argc;
    init_setup(&shell, argv, envp);
    while (!shell.should_exit)
    {
        dyn_str_init(&shell.input);
        get_g_sig()->should_unwind = 0;
        parse_and_execute_input(&shell);
        manage_history(&shell.rl, &shell.hist);
        free(shell.input.buff);
        shell.input = (t_dyn_str){};
    }
    free_env(&shell.env);
    free_all_state(&shell);
    forward_exit_status(shell.last_cmd_status_res);
}