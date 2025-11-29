/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_var.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:10:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 15:41:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATE_VAR_H
# define PRIVATE_VAR_H

/*
 * External headers required for types used in the prototypes.
 */
# include <stddef.h>
# include <inttypes.h>
# include <string.h>
# include "configs.h"
# include "ft_stddef.h"
/*
 * Option list size (must be defined before struct s_var_state uses it)
 */
# ifndef NOPTS
#  define NOPTS 64
# endif

/*
 * Hash table size
 */
/* if defined, use the writable canonical empty string from mystring.c */
#ifdef MUTABLE_DEPENDANCE
extern char nullstr[1];   /* declared in mystring.c */
#define NULLSTR (nullstr)  /* expands to a char* */
#else
/* default: use a string literal (read-only) */
#define NULLSTR ("")
#endif


# define DEFPATHVAR "PATH=/usr/local/sbin:/usr/local/bin:\
					/usr/sbin:/usr/bin:/sbin:/bin"
# define DEFIFSVAR "IFS= \t\n"
# define DEFOPTINDVAR "OPTIND=1"
/*
 * Variable flags
 */
enum e_vflag
{
	VEXPORT = 0x01,
	VREAD_ONLY = 0x02,
	VSTR_FIXED = 0x04,
	VTEXT_FIXED = 0x08,
	VSTACK = 0x10,
	VUNSET = 0x20,
	VNO_FUNC = 0x40,
	VNO_SET = 0x80,
	VNO_SAVE = 0x100,
	VTABSIZE = 39
};

/**
 * @brief Represents a single variable in the hash table.
 */
typedef struct s_var
{
	struct s_var	*next;
	int				flags;
	const char		*text;
	void			(*func)(const char *);
}	t_var;

/**
 * @brief Represents a saved variable state for 'local' command.
 */
typedef struct s_localvar
{
	struct s_localvar	*next;
	t_var				*vp;
	int					flags;
	const char			*text;
}	t_localvar;

/**
 * @brief A node in the stack of local variable scopes.
 */
typedef struct s_localvar_list
{
	struct s_localvar_list	*next;
	t_localvar				*lv;
}	t_localvar_list;

/**
 * @brief Opaque structure for the singleton state.
 * The full definition is private to 'var_state.c'.
 */
typedef struct s_var_state	t_var_state;

// Minimal public struct for code that needs vartab access
typedef struct s_var_state
{
	struct s_var			*vartab[VTABSIZE];
	struct s_localvar_list	*localvar_stack;
	char					optlist[NOPTS];
	struct s_var			varinit[16];
	int						varinit_size;
	const char				*defpathvar;
	const char				*defifsvar;
	const char				*defoptindvar;
	char					linenovar[16];
	struct s_var			*vlineno_ptr;
	char					oplist[NOPTS];
	int						lineno;
}	t_var_state;

typedef struct s_meta
{
	const char	*name;
	size_t		len;
}	t_meta;

/*
 * TEST UTILITY: Do not use in production.
 * Resets the singleton state for unit testing.
 */
void			libvar_reset_state(void);
intmax_t		ft_atomax(const char *s, int base);
t_var			make_atty(void);
int				add_atty(t_var_state *state, int i);
t_var			make_ifs(const char *ifs);
t_var			make_mail(void);
t_var			make_mailpath(void);
t_var			make_path(const char *path);
t_var			make_optind(const char *optind, int (*reset)(void));
t_var			make_lineno(const char *lineno);
t_var			make_term(void);
t_var			make_histsize(void);
t_var			make_ps4(void);
int				add_term_histsize(t_var_state *state, int i);
int				init_varinit_pt2(t_var_state *state, int i);
t_var			make_ps1(void);
t_var			make_ps2(void);
t_var			**find_var(const char *name);

/********************
 * * Accessors
 ********************/

/**
 * @brief Gets the singleton instance of the variable state.
 * @return A pointer to the unique t_var_state struct.
 */
t_var_state		*get_var_state(void);
t_localvar_list	*get_localvar_stack(void);
char			*get_optlist(void);
#endif
