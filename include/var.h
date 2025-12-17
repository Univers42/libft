/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:07:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 01:36:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VAR_H
# define VAR_H

#include <stddef.h>
#include <inttypes.h>
#include <string.h>
#include "ds.h"
#include "private_var.h"

typedef struct s_var			t_var;
typedef struct s_localvar		t_localvar;
typedef struct s_localvar_list	t_localvar_list;
typedef struct s_var_state		t_var_state;
typedef struct s_meta			t_meta;

/*********************
 * PUBLIC APIS
 *********************/

/**
 * @brief Sets or unsets a variable.
 * @param name The name of the variable to set.
 * @param val The value to set. If NULL, the variable is unset.
 * @param flags Flags to be OR'ed with the variable's flags (e.g., VEXPORT).
 * @return A pointer to the variable structure.
 */
t_var			*set_var(const char *name, const char *val, int flags);

/**
 * @brief Sets a variable to an integer value.
 * @param name The name of the variable to set.
 * @param val The integer (intmax_t) value.
 * @param flags Flags to be OR'ed with the variable's flags.
 * @return The integer value that was set.
 */
intmax_t		set_varint(const char *name, intmax_t val, int flags);

/**
 * @brief Sets a variable from a "name=value" string.
 * @param s The string in "name=value" format.
 * @param flags Flags to be OR'ed with the variable's flags.
 * @return A pointer to the variable structure.
 */
t_var			*set_vareq(char *s, int flags);

/**
 * @brief Looks up the value of a variable.
 * @param name The name of the variable.
 * @return The value of the variable as a string, or NULL if not set.
 */
char			*lookup_var(const char *name);

/**
 * @brief Looks up the value of a variable and converts it to an integer.
 * @param name The name of the variable.
 * @return The integer value (intmax_t), or 0 if not set or not a number.
 */
intmax_t		lookup_varint(const char *name);

/**
 * @brief Unsets the specified variable.
 * @param s The name of the variable to unset.
 */
void			unset_var(const char *s);

/*
 * Built-in Command Implementations
 */

/**
 * @brief Implementation of the 'export' and 'readonly' built-in commands.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success.
 */
int				export_cmd(int argc, char **argv);

/**
 * @brief Implementation of the 'local' built-in command.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success.
 */
int				local_cmd(int argc, char **argv);

/**
 * @brief Implementation of the 'unset' built-in command.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success.
 */
int				unset_cmd(int argc, char **argv);

/**
 * @brief Prints variables in lexicographic order, suitable for 'set'.
 * @param prefix A string to print before each variable (e.g., "export").
 * @param on A bitmask of flags that must be ON.
 * @param off A bitmask of flags that must be OFF.
 * @return 0 on success.
 */
int				show_vars(const char *prefix, int on, int off);

/*
 * Local Scope (Function) Management
 */

/**
 * @brief Makes a variable local to the current function scope.
 * @param name The name of the variable (can be "name=value").
 * @param flags Additional flags to apply.
 */
void			mklocal(char *name, int flags);

/**
 * @brief Pushes a new local variable scope onto the stack.
 * @param push 1 to push a new scope, 0 to just return the current top.
 * @return The top of the local variable stack (before pushing, if push=1).
 */
t_localvar_list	*push_local_vars(int push);

/**
 * @brief Pops the current local variable scope from the stack.
 */
void			pop_local_vars(void);

/**
 * @brief Unwinds the local variable stack down to a specific level.
 * @param stop The stack level to stop at (returned by a previous pushlocalvars).
 */
void            unwind_local_vars(t_localvar_list *stop);

/*
 * List & Helper Functions
 */

/**
 * @brief Generates a list of variables matching flag conditions.
 * @param on A bitmask of flags that must be ON.
 * @param off A bitmask of flags that must be OFF.
 * @param end A pointer that will be set to the end of the returned list.
 * @return An array of strings ("name=value"), NULL-terminated.
 */
char			**list_vars(int on, int off, char ***end);

/**
 * @brief (Was static) Hashes a variable name.
 * @param p The variable name.
 * @return A pointer to the bucket in the state->vartab.
 */
t_var			**var_hash(const char *p);

/**
 * @brief (Was static) Finds a variable in a hash bucket.
 * @param vpp Pointer to the hash bucket (from var_hash).
 * @param name The name of the variable to find.
 * @return &vartab[index]
 * @note the loop wals the linked list of struct var object
 * in that bucket, comparing names.
 * @note the idea behind this var_find is that if it fidns a match
 * it returns a pointer to that struct var pointer; otherwise
 * it returns the pointer where a new entry would be inserted
 */
t_var           **var_find(struct s_var **vpp, const char *name);

/**
 * @brief (Was static) Finds a variable in a hash bucket.
 * @param vpp Pointer to the hash bucket (from var_hash).
 * @param name The name of the variable to find.
 * @return &vartab[index]
 * @note the loop wals the linked list of struct var object
 * in that bucket, comparing names.
 * @note the idea behind this var_find is that if it fidns a match
 * it returns a pointer to that struct var pointer; otherwise
 * it returns the pointer where a new entry would be inserted
 */
t_var	**find_var(const char *name);

/**
 * @brief (Was 'varcmp') Compares two variable names (up to '=' or '\0').
 * @param p First string (must end in '=' or '\0').
 * @param q Second string (must end in '=' or '\0').
 * @return An integer <, ==, or > 0.
 */
int	            var_cmp(const char *p, const char *q);

/**
 * @brief (Was 'vpcmp') Comparison function for qsort (uses libvar_varcmp).
 * @param a Pointer to the first string.
 * @param b Pointer to the second string.
 * @return An integer <, ==, or > 0.
 */
int				var_vpcmp(const void *a, const void *b);
char *env_expand_n(char *last_cmd_status_s, char *pid, char *key, int len, t_vec *env);




#endif
