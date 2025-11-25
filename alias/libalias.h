#ifndef LIBALIAS_H
# define LIBALIAS_H

# include "libft.h"

# define ALIAS_TAB_SIZE 127

# define ALIAS_INUSE	1
# define ALIAS_DEAD		2

typedef struct s_alias
{
	struct s_alias	*next;
	char			*name;
	char			*val;
	int				flag;
}					t_alias;

typedef struct s_alias_state
{
	t_alias			*atab[ALIAS_TAB_SIZE];
}					t_alias_state;

/* Singleton State */

/**
 * @brief Retrieves the singleton instance of the alias state.
 * @return A pointer to the unique static t_alias_state structure.
 */
t_alias_state		*get_alias_state(void);

/* API Core */

/**
 * @brief Locates the slot (pointer to pointer) for a 
 * given alias name in the hash table.
 * Computes the hash of the name and traverses the 
 * bucket to find where the alias resides
 * or where it should be inserted.
 * @param name The alias name to hash and locate.
 * @return A pointer to the `t_alias *` holding 
 * the node (or the NULL terminator).
 */
t_alias				**alias_find_slot(const char *name);

/**
 * @brief Searches for an alias structure by name.
 * @param name The name of the alias to find.
 * @param check If set to 1, aliases marked as 
 * ALIAS_INUSE are ignored (returns NULL)
 * to prevent infinite recursion during alias expansion.
 * @return A pointer to the found t_alias node, or NULL if not found.
 */
t_alias				*alias_lookup(const char *name, int check);

/**
 * @brief Creates a new alias or updates an existing one.
 * If the alias exists, its value is updated 
 * (unless it's currently in use).
 * If it doesn't exist, a new node is allocated and added to the table.
 * @param name The name of the alias.
 * @param val The value (command string) assigned to the alias.
 */
void				alias_set(const char *name, const char *val);

/**
 * @brief Removes a specific alias by name.
 * @param name The name of the alias to remove.
 * @return 0 if the alias was successfully
 * removed, 1 if it was not found.
 */
int					alias_unset(const char *name);

/**
 * @brief Removes all aliases from 
 * the hash table (equivalent to `unalias -a`).
 * Iterates through the entire table and frees every node.
 */
void				alias_remove_all(void);

/**
 * @brief Frees an alias node's memory.
 * Handles the ALIAS_INUSE flag: if the alias
 * is currently executing, it is marked
 * as ALIAS_DEAD instead of being freed 
 * immediately to avoid use-after-free.
 * @param ap Pointer to the alias node to free.
 * @return A pointer to the next node in 
 * the list (ap->next), allowing seamless list traversal.
 */
t_alias				*alias_free(t_alias *ap);

/**
 * @brief Prints an alias definition to standard 
 * output in the format `name='value'`.
 * The value is properly quoted using single quotes.
 * @param ap Pointer to the alias node to print.
 */
void				alias_print(const t_alias *ap);

/* Builtins */

/**
 * @brief Implementation of the `alias` builtin command.
 * Without arguments, prints all aliases.
 * With arguments, prints specific aliases
 * or assigns new ones (`name=value`).
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status (0 on success, >0 on error).
 */
int					alias_cmd(int argc, char **argv);

/**
 * @brief Implementation of the `unalias` builtin command.
 * Supports the `-a` flag to remove all aliases.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status (0 on success, >0 on error).
 */
int					unalias_cmd(int argc, char **argv);

/* Auxiliares */

/**
 * @brief Escapes single quotes in a string for shell-safe printing.
 * Converts a string like "don't" into "'don'\''t'".
 * @param s The input string to quote.
 * @return A new allocated string enclosed in single quotes
 * with internal quotes escaped.
 */
char				*alias_str_quote(const char *s);

#endif
