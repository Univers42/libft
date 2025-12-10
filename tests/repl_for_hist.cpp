/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_for_hist.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:32:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/10 21:01:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern "C"
{
#include "ft_readline.h"
#include "var.h"
#include "ds.h"
}

int main()
{
	t_vec env;
	t_vec_config env_config = {.elem_size = sizeof(t_env), .initial_capacity = 4, .type_mask = 0, .copy_fn = NULL, .free_fn = NULL};
	t_status status = {0, -1, false};
	char *status_str = NULL;
	int input_method = INP_READLINE;
	char *context = NULL;
	const char *base_context_str = "shell";
	char *base_context = const_cast<char *>(base_context_str);
	t_repl_config config;

	// Initialize a dummy environment vector for history (add HOME for history file path)
	vec_init(&env, &env_config);
	const char *home_key = "HOME";
	const char *home_value = "/tmp";
	t_env home_env = {false, const_cast<char *>(home_key), const_cast<char *>(home_value)}; // Dummy HOME for testing
	vec_push(&env, &home_env);

	// Configure the REPL
	config.env = &env;
	config.base_context = base_context;
	config.context = &context;
	config.input_method = input_method;
	config.status = &status;
	config.status_str = &status_str;
	config.basic_prompt = "hello";
	config.multi_line_prompt = NULL;
	config.dquote_prompt = NULL;
	config.squote_prompt = NULL;
	config.prompt_gen = NULL;	 // Use default prompt generator
	config.process_input = NULL; // Use default input processor (echo and check for "exit")
	config.cleanup = NULL;		 // No custom cleanup
	config.echo_input = true;
	config.handle_signals = true;
	config.enable_history = true;
	config.accumulate_continuation = true;
	config.user_data = NULL;

	// Run the REPL
	int exit_code = repl_run(&config);

	// Cleanup
	free(status_str);
	free(context);
	vec_destroy(&env);

	return exit_code;
}
