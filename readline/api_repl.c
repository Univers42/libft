/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_repl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:52:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/10 21:05:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdbool.h>
#include "ft_stdio.h"

/* Default input processor: echo and check for "exit" */
static bool default_process_input(const char *input, t_status *status)
{
    if (!status)
        return false;
    if (ft_strcmp(input, "exit") == 0)
    {
        status->status = 0;
        return true; /* Exit */
    }
    ft_printf("Echo: %s\n", input);
    status->status = 0;
    return false; /* Continue */
}

/* Main REPL runner function */
int repl_run(t_repl_config *config)
{
    t_rl rl;
    t_hist hist;
    t_dyn_str input;
    t_dyn_str prompt;
    char *prompt_str;
    int ret;
    bool should_exit = false;

    if (!config || !config->status)
        return -1;

    /* Initialize readline and history if enabled */
    buff_readline_init(&rl);
    dyn_str_init(&input);
    if (config->enable_history)
        init_history(&hist, config->env);

    while (!should_exit)
    {
        /* Generate prompt: prefer prompt_gen, else basic_prompt (if set), else default PROMPT */
        if (config->prompt_gen)
        {
            prompt = config->prompt_gen(config->status, config->status_str);
        }
        else
        {
            dyn_str_init(&prompt);
            if (config->basic_prompt && config->basic_prompt[0] != '\0')
            {
                /* Use exact basic_prompt content. Do not prepend/replace it with '>'.
                   Add a single trailing space if the prompt doesn't already end with whitespace,
                   to separate prompt text from user input. */
                dyn_str_pushstr(&prompt, config->basic_prompt);
                size_t lp = ft_strlen(config->basic_prompt);
                if (lp > 0 && config->basic_prompt[lp - 1] != ' ' && config->basic_prompt[lp - 1] != '\t')
                    dyn_str_push(&prompt, ' ');
            }
            else
            {
                /* Fallback to PROMPT macro (keeps previous appearance including markers) */
                dyn_str_pushstr(&prompt, PROMPT);
            }
        }
        prompt_str = prompt.buff;

        /* Read input */
        ret = xreadline(&rl, &input, prompt_str, config->status, config->status_str,
                        &config->input_method, config->context, &config->base_context);

        /* free prompt buffer created by dyn_str */
        free(prompt_str);

        if (ret == 0)
        {
            /* EOF */
            should_exit = true;
            continue;
        }
        if (ret == 2 && config->handle_signals)
        {
            /* Interrupt */
            if (config->echo_input)
                ft_printf("\n");
            config->status->status = 130;
            config->status->c_c = true;
            dyn_str_clear(&input);
            if (config->enable_history)
                manage_history(&hist, &rl);
            continue;
        }

        /* Handle backslash continuation if enabled */
        if (config->accumulate_continuation &&
            input.len > 0 && input.buff[input.len - 1] == '\n' &&
            input.len > 1 && input.buff[input.len - 2] == '\\')
        {
            input.len -= 2;
            input.buff[input.len] = '\0';
            continue;
        }

        /* Remove trailing newline */
        if (input.len > 0 && input.buff[input.len - 1] == '\n')
            input.buff[input.len - 1] = '\0';

        /* Process input */
        if (config->process_input)
            should_exit = config->process_input(input.buff, config->status);
        else
            should_exit = default_process_input(input.buff, config->status);

        /* Manage history if enabled */
        if (config->enable_history)
        {
            dyn_str_clear(&rl.str);
            dyn_str_pushstr(&rl.str, input.buff);
            dyn_str_push(&rl.str, '\n');
            rl.cursor = rl.str.len;
            manage_history(&hist, &rl);
        }

        /* Clear input for next iteration */
        dyn_str_clear(&input);
    }

    /* Cleanup */
    if (config->enable_history)
        free_hist(&hist);
    if (config->cleanup)
        config->cleanup(config->user_data);

    return config->status->status;
}