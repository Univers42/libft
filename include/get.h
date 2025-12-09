/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:27:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 17:57:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_H
#define GET_H

#include "libft.h"
#include "alias.h"
#include "trap.h"
#include "arena.h"
#include "array.h"
#include "geometry.h"
#include "parser.h"

/* ========================================================================== */
/* Master Singleton Structure - Central Access Point                         */
/* ========================================================================== */

typedef struct s_get_singletons
{
	/* Alias management */
	t_alias_state		*alias_state;
	/* Signal/Trap handling */
	t_glob_sig			*glob_sig;
	/* Memory arena */
	t_arena				*arena;
	t_garena			*garena;
	/* Array/List structures */
	t_arr				*arr;
	/* Geometry types */
	t_pos				pos;
	t_vec2				vec2d;
	t_position			position;
	t_sphere_data		sphere;
	/* Parser state */
	t_parse				*parse;
	/* Line geometry */
	t_line2				line2d;
	t_line3				line3d;
	/* Variable management */
	t_var_state			*var_state;
	/* Input handling */
	t_input				*input;
	/* Error state */
	t_error_state		*error_state;
	/* Output context */
	t_out_ctx			*out_ctx;
	/* RNG state */
	t_rng_state			*rng_state;
	/* History */
	t_hist				*hist;
	/* Readline state */
	t_rl				*rl;
	/* Termination state */
	struct s_termg		*termg;
	t_dyn_str			*str;
	t_error_state		*estate;
	t_state_fd			*fd;
	t_circular_list		*clist;
	t_doubly_list		*dlist;
	t_hash_seen			*bitmap;
	t_hash_entry		*entry;
	t_hash_config		*hash_config;
	t_hash				*hash;
	t_list				*list;
	t_queue				*queue;
	t_deque				*deque;
	t_vec_config		*vec_config;
	t_vec				*vec;
	t_map_token			*map_token;
	t_tok				*tok;
	t_token				*token;
	t_input_source		*input_source;
	t_input_stream		*t_input_stream;
	t_delim_stack		*stack;
	t_op_map			*op_map;
	t_reserved_word		*reserved_word;
	t_keyword_entry		*keyword_entry;
	t_scanner			*scanner;
	t_dispatch_data		*dispatch_data;
	t_stack_block		*block;
	t_stack_mark		*mark;
	t_garena			*garena;
	t_dynall			*dynall;
	t_pipe_fds			*pipe_fds;
	t_pool				*pool;
	t_slab_allocator	*slab_allocator;
	t_slab_chunk		*slab_chunk;
	t_slab_cache		*cache;
	t_glob_sig			*watcher_signal;
	t_localvar			*localvar;
	t_var_state			*var_state;
	t_meta				*meta_var;
	t_localvar_list		*localvar_list;
} t_get_singletons;

/* ========================================================================== */
/* Singleton Accessor Function                                              */
/* ========================================================================== */

/**
 * @brief Retrieves the global singleton instance of all singletons.
 *
 * This function provides access to a centralized structure that holds
 * pointers to all major singleton objects in the application.
 * Use this to access: get()->alias_state, get()->glob_sig, etc.
 *
 * @return Pointer to the unique static t_get_singletons structure.
 *
 * @note The returned structure is persistent for the lifetime of the program.
 *       All pointers within may be NULL if their subsystems haven't been
 *       initialized yet.
 *
 * @example
 *   t_get_singletons *g = get();
 *   t_alias_state *aliases = g->alias_state;
 *   t_glob_sig *signals = g->glob_sig;
 */
t_get_singletons *get(void);

/* ========================================================================== */
/* Initialization & Cleanup                                                  */
/* ========================================================================== */

/**
 * @brief Initialize all singleton subsystems.
 *
 * Called once at program startup to set up arena, aliases, signals,
 * variables, input handling, and other core subsystems.
 *
 * @return 0 on success, non-zero on failure.
 */
int init_all_singletons(void);

/**
 * @brief Clean up and destroy all singleton subsystems.
 *
 * Called at program exit to free arena blocks, close files, and reset
 * all singleton pointers. Must be called before process termination.
 */
void destroy_singletons(void);

/* ========================================================================== */
/* Convenience Accessors (Optional inline helpers)                           */
/* ========================================================================== */

/**
 * @brief Quick accessor for alias state.
 * @return Pointer to t_alias_state singleton, or NULL if not initialized.
 */
static inline t_alias_state *get_aliases(void)
{
	t_get_singletons *g = get();
	return g ? g->alias_state : NULL;
}

/**
 * @brief Quick accessor for signal/trap state.
 * @return Pointer to t_glob_sig singleton, or NULL if not initialized.
 */
static inline t_glob_sig *get_signals(void)
{
	t_get_singletons *g = get();
	return g ? g->glob_sig : NULL;
}

/**
 * @brief Quick accessor for memory arena.
 * @return Pointer to t_arena singleton, or NULL if not initialized.
 */
static inline t_arena *get_arena_block(void)
{
	t_get_singletons *g = get();
	return g ? g->arena : NULL;
}

/**
 * @brief Quick accessor for LIFO stack arena.
 * @return Pointer to t_garena singleton, or NULL if not initialized.
 */
static inline t_garena *get_stack_arena(void)
{
	t_get_singletons *g = get();
	return g ? g->garena : NULL;
}

/**
 * @brief Quick accessor for variable state.
 * @return Pointer to t_var_state singleton, or NULL if not initialized.
 */
static inline t_var_state *get_vars(void)
{
	t_get_singletons *g = get();
	return g ? g->var_state : NULL;
}

/**
 * @brief Quick accessor for input state.
 * @return Pointer to t_input singleton, or NULL if not initialized.
 */
static inline t_input *get_stdin(void)
{
	t_get_singletons *g = get();
	return g ? g->input : NULL;
}

/**
 * @brief Quick accessor for error state.
 * @return Pointer to t_error_state singleton, or NULL if not initialized.
 */
static inline t_error_state *get_errors(void)
{
	t_get_singletons *g = get();
	return g ? g->error_state : NULL;
}

/**
 * @brief Quick accessor for output context.
 * @return Pointer to t_out_ctx singleton, or NULL if not initialized.
 */
static inline t_out_ctx *get_outputs(void)
{
	t_get_singletons *g = get();
	return g ? g->out_ctx : NULL;
}

#endif