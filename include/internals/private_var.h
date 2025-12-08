/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_var.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:10:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/08 01:31:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATE_VAR_H
#define PRIVATE_VAR_H

/* Forward declarations to avoid circular dependencies */
typedef struct s_hellish t_hellish;
typedef struct s_localvar t_localvar;
typedef struct s_localvar_list t_localvar_list;
typedef struct s_var_state t_var_state;
typedef struct s_var t_var;
/* Public header provides canonical typedefs */


/* avoid including ft_stdio.h here when compiling as C++ (these headers
   contain C-only constructs that are not C++-friendly). Include them
   only for C builds. */
#ifdef __cplusplus
extern "C"
{
#endif

#include "var.h"
#include "configs.h"
#include "ft_string.h"
#include "ft_memory.h"
#include <stddef.h>
#include <inttypes.h>
#include <stdlib.h>
#include "ds.h"
#include "ft_stdio.h"
#include "ft_sort.h"

/* If the public headers did not define reasonable sizes, provide safe defaults.
   Use guards so we don't clobber upstream definitions. */
#ifndef VTABSIZE
#define VTABSIZE 127
#endif

#ifndef NOPTS
#define NOPTS 32
#endif

/* Provide an internal definition of struct s_var so internal modules can
   allocate arrays of struct s_var (e.g. varinit[16]). We do NOT create a
   typedef here (var.h already declares 'typedef struct s_var t_var;'). */
struct s_var
{
	struct s_var *next;
	int flags;
	const char *text;
	void (*func)(const char *);
};

/* Minimal metadata structure used by set_var/build helpers.
   The public header forward-declares 'typedef struct s_meta t_meta;'
   but the concrete struct layout is private and required by implementation
   files (set_var.c, etc.). Provide it here (no typedef duplication). */
struct s_meta
{
	const char *name;
	size_t len;
};

typedef struct s_env
{
	bool exported;
	char *key;
	char *value;
} t_env;

/* ---- Missing flag macros (guarded) ----
   Provide reasonable defaults only if not defined by public headers.
*/
#ifndef VEXPORT
#define VEXPORT 0x01
#endif
#ifndef VREAD_ONLY
#define VREAD_ONLY 0x02
#endif
#ifndef VSTR_FIXED
#define VSTR_FIXED 0x04
#endif
#ifndef VTEXT_FIXED
#define VTEXT_FIXED 0x08
#endif
#ifndef VSTACK
#define VSTACK 0x10
#endif
#ifndef VUNSET
#define VUNSET 0x20
#endif
#ifndef VNO_FUNC
#define VNO_FUNC 0x40
#endif
#ifndef VNO_SET
#define VNO_SET 0x80
#endif
#ifndef VNO_SAVE
#define VNO_SAVE 0x100
#endif

/* If code uses option indices such as AFLAG, ensure a guarded definition
   so translation units referencing get_optlist()[AFLAG] can compile.
   Adjust the numeric value to match your option-table layout if needed. */
#ifndef AFLAG
#define AFLAG 10
#endif

/* Provide default string macros used during var_state initialization.
   Allow the build system or other headers to override these by defining
   them before including this header. */
#ifndef DEFPATHVAR
#define DEFPATHVAR "PATH=/bin:/usr/bin"
#endif

#ifndef DEFIFSVAR
/* IFS default: space, tab, newline */
#define DEFIFSVAR "IFS= \t\n"
#endif

#ifndef DEFOPTINDVAR
#define DEFOPTINDVAR "OPTIND=1"
#endif


/* Internal definitions for local-variable structures used by implementation.
   We declare struct s_localvar / s_localvar_list (no typedef) so .c files can
   allocate and reference their fields; the public header keeps typedef names
   (t_localvar, t_localvar_list) and will refer to these structs. */
struct s_localvar
{
	struct s_localvar *next;
	struct s_var *vp;
	int flags;
	const char *text;
	void (*restore)(struct s_localvar *lvp, t_var_state *state);
};

struct s_localvar_list
{
	struct s_localvar_list *next;
	t_localvar *lv;
};

/* -------------------------------------------------------------------------
   INTERNAL: define struct s_var_state so internal implementation files
   may access its fields. Do NOT create a typedef here — var.h already
   provides "typedef struct s_var_state t_var_state;" and that typedef
   will name this struct correctly.
   ------------------------------------------------------------------------- */
struct s_var_state
{
	/* hash table of variables */
	struct s_var *vartab[VTABSIZE];

	/* stack of local variable scopes */
	struct s_localvar_list *localvar_stack;

	/* option lists (canonical storage) */
	char optlist[NOPTS];
	char oplist[NOPTS];

	/* initial set of variables used during initialization */
	struct s_var varinit[16];
	int varinit_size;

	/* defaults for a few special variables (point to static strings) */
	const char *defpathvar;
	const char *defifsvar;
	const char *defoptindvar;

	/* line-number helper (optional) */
	char linenovar[16];
	struct s_var *vlineno_ptr;
	int lineno;
};

/* ---- Private-only helpers and test utilities ----
   Expose only additional internal function prototypes needed across
   the var implementation. Do NOT redefine public structs/macros.
*/

/* Test utility: reset singleton state for unit tests. */
void libvar_reset_state(void);

/* Numeric helper */
intmax_t ft_atomax(const char *s, int base);

/* Factory helpers used by var_state init */
t_var make_atty(void);
t_var make_ifs(const char *ifs);
t_var make_mail(void);
t_var make_mailpath(void);
t_var make_path(const char *path);
/* change: reset callback returns void (matches var_state helpers) */
t_var make_optind(const char *optind, void (*resetfn)(void));
t_var make_lineno(const char *lineno);
t_var make_term(void);
t_var make_histsize(void);
t_var make_ps1(void);
t_var make_ps2(void);
t_var make_ps4(void);

/* var_state helpers used during initialization */
int add_atty(t_var_state *state, int i);
int add_term_histsize(t_var_state *state, int i);
int init_varinit_pt2(t_var_state *state, int i);

/* var lookup helpers provided privately */
t_var **find_var(const char *name);
char *var_null(const char *s);

/* DECLARE unset_func used by unset.c (stub / shell integration) */
void unset_func(const char *name);

/* Accessors that are implemented in var/ (kept for internal linkage) */
t_var_state *get_var_state(void);
t_localvar_list *get_localvar_stack(void);
char *get_optlist(void);
intmax_t set_varint(const char *name, intmax_t val, int flags);

/* New: teardown API to free all libvar-managed allocations */
void libvar_destroy(void);

/* Environment management functions */
t_env str_to_env(char *str);
char **get_envp(t_vec *env);
void free_env(t_vec *env);
t_env *env_get(t_vec *env, char *key);
char *env_get_ifs(t_vec *env);
char *env_expand(char *last_cmd_status_s, char *pid, char *key, t_vec *env);
int env_set(t_vec *env, t_env new_entry);
void env_extend(t_vec *edst, t_vec *esrc, bool do_export);
t_env *env_nget(t_vec *env, char *key, int len);
t_vec create_vec_env(char **envp);
#ifdef __cplusplus
}
#endif
#endif
