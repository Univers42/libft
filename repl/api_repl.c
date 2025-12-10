/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_repl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:52:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/10 20:58:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdbool.h>
#include "ft_stdio.h"


// Default prompt generator using the config's prompts
static t_dyn_str default_prompt_gen(t_status *status, char **status_str)
{
    (void)status_str;
    t_dyn_str p;
    dyn_str_init(&p);
    dyn_str_pushstr(&p, (status->status == 0) ? "> " : "error> ");
    return p;
}

// Default input processor: echo and check for "exit"
static bool default_process_input(const char *input, t_status *status)
{
    if (ft_strcmp(input, "exit") == 0)
    {
        status->status = 0;
        return true; // Exit
    }
    ft_printf("Echo: %s\n", input);
    status->status = 0;
    return false; // Continue
}

// Main REPL runner function
int repl_run(t_repl_config *config)
{
    t_rl rl;
    t_hist hist;
    t_dyn_str input;
    char *prompt_str;
    int ret;
    bool should_exit = false;

    // Initialize readline and history if enabled
    buff_readline_init(&rl);
    dyn_str_init(&input);
    if (config->enable_history)
    {
        init_history(&hist, config->env);
    }

    while (!should_exit)
    {
        // Generate prompt
        t_dyn_str prompt;
        if (config->prompt_gen)
        {
            prompt = config->prompt_gen(config->status, config->status_str);
        }
        else
        {
            prompt = default_prompt_gen(config->status, config->status_str);
        }
        prompt_str = prompt.buff;

        // Read input
        ret = xreadline(&rl, &input, prompt_str, config->status, config->status_str,
                        &config->input_method, config->context, &config->base_context);

        free(prompt_str); // Free the prompt string

        if (ret == 0)
        {
            // EOF
            should_exit = true;
            continue;
        }
        if (ret == 2 && config->handle_signals)
        {
            // Interrupt
            if (config->echo_input)
                ft_printf("\n");
            config->status->status = 130;
            config->status->c_c = true;
            dyn_str_clear(&input);
            if (config->enable_history)
                manage_history(&hist, &rl);
            continue;
        }

        // Handle backslash continuation if enabled
        if (config->accumulate_continuation &&
            input.len > 0 && input.buff[input.len - 1] == '\n' &&
            input.len > 1 && input.buff[input.len - 2] == '\\')
        {
            input.len -= 2;
            input.buff[input.len] = '\0';
            continue;
        }

        // Remove trailing newline
        if (input.len > 0 && input.buff[input.len - 1] == '\n')
            input.buff[input.len - 1] = '\0';

        // Process input
        if (config->process_input)
        {
            should_exit = config->process_input(input.buff, config->status);
        }
        else
        {
            should_exit = default_process_input(input.buff, config->status);
        }

        // Manage history if enabled
        if (config->enable_history)
        {
            // Prepare rl->str for history
            dyn_str_clear(&rl.str);
            dyn_str_pushstr(&rl.str, input.buff);
            dyn_str_push(&rl.str, '\n');
            rl.cursor = rl.str.len;
            manage_history(&hist, &rl);
        }

        // Clear input for next iteration
        dyn_str_clear(&input);
    }

    // Cleanup
    if (config->enable_history)
        free_hist(&hist);
    if (config->cleanup)
        config->cleanup(config->user_data);

    return config->status->status;
}