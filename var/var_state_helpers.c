/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 16:04:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 01:44:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

extern int prefix(const char *s, const char *p); /* forward decl to avoid implicit decl */

/* Use forward declarations (incomplete types) so we can reference these names
   in pointer fields before we provide the full definitions later in this file. */
union u_shnode;
struct s_shnode_list;
struct s_ftnode;

/* Minimal built-in command descriptor used by the command-table union. */
typedef struct s_builtincmd
{
	const char *name;
	int (*builtin)(int, char **);
	unsigned flags;
} t_builtincmd;

/* Parameter union for command-table entries (matches original layout). */
typedef union u_param
{
	int idx;
	const t_builtincmd *cmd;
	struct s_ftnode *fn;
} t_param;

#define ALIASCMD (builtincmd + 3)
#define BGCMD (builtincmd + 4)
#define BREAKCMD (builtincmd + 5)
#define CDCMD (builtincmd + 6)
#define COMMANDCMD (builtincmd + 8)
#define DOTCMD (builtincmd + 0)
#define ECHOCMD (builtincmd + 10)
#define EVALCMD (builtincmd + 11)
#define EXECCMD (builtincmd + 12)
#define EXITCMD (builtincmd + 13)
#define EXPORTCMD (builtincmd + 14)
#define FALSECMD (builtincmd + 15)
#define FGCMD (builtincmd + 16)
#define GETOPTSCMD (builtincmd + 17)
#define HASHCMD (builtincmd + 18)
#define JOBSCMD (builtincmd + 19)
#define KILLCMD (builtincmd + 20)
#define LOCALCMD (builtincmd + 21)
#define PRINTFCMD (builtincmd + 22)
#define PWDCMD (builtincmd + 23)
#define READCMD (builtincmd + 24)
#define RETURNCMD (builtincmd + 26)
#define SETCMD (builtincmd + 27)
#define SHIFTCMD (builtincmd + 28)
#define TESTCMD (builtincmd + 2)
#define TIMESCMD (builtincmd + 30)
#define TRAPCMD (builtincmd + 31)
#define TRUECMD (builtincmd + 1)
#define TYPECMD (builtincmd + 33)
#define ULIMITCMD (builtincmd + 34)
#define UMASKCMD (builtincmd + 35)
#define UNALIASCMD (builtincmd + 36)
#define UNSETCMD (builtincmd + 37)
#define WAITCMD (builtincmd + 38)

#define NUMBUILTINS 39

#define BUILTIN_SPECIAL 0x1
#define BUILTIN_REGULAR 0x2
#define BUILTIN_ASSIGN 0x4

#define CMDTABLESIZE 31
#define ARB 1

#define NCMD 0
#define NPIPE 1
#define NREDIR 2
#define NBACKGND 3
#define NSUBSHELL 4
#define NAND 5
#define NOR 6
#define NSEMI 7
#define NIF 8
#define NWHILE 9
#define NUNTIL 10
#define NFOR 11
#define NCASE 12
#define NCLIST 13
#define NDEFUN 14
#define NARG 15
#define NTO 16
#define NCLOBBER 17
#define NFROM 18
#define NFROMTO 19
#define NAPPEND 20
#define NTOFD 21
#define NFROMFD 22
#define NHERE 23
#define NXHERE 24
#define NNOT 25

#define CMDUNKNOWN -1 /* no entry in table for command */
#define CMDNORMAL 0	  /* command is an executable program */
#define CMDFUNCTION 1 /* command is a shell function */
#define CMDBUILTIN 2  /* command is a shell builtin */

typedef struct s_ncmd
{
	int type;
	int lin_no;
	union u_shnode *assign;
	union u_shnode *args;
	union u_shnode *redirect;
} t_ncmd;

typedef struct s_npipe
{
	int type;
	int bg;
	struct s_shnode_list *cmd_list;
} t_npipe;

typedef struct s_redir
{
	int type;
	int lin_no;
	union u_shnode *n;
	union u_shnode *redirect;
} t_nredir;

typedef struct s_nbinary
{
	int type;
	union u_shnode *ch1;
	union u_shnode *ch2;
} t_nbinary;

typedef struct s_nif
{
	int type;
	union u_shnode *test;
	union u_shnode *if_part;
	union u_shnode *else_part;
} t_nif;

typedef struct s_nfor
{
	int type;
	int line_no;
	union u_shnode *args;
	union u_shnode *body;
	char *var;
} t_nfor;

typedef struct s_ncase
{
	int type;
	int line_no;
	union u_shnode *expr;
	union u_shnode *cases;
} t_ncase;

typedef struct s_nclist
{
	int type;
	union u_shnode *next;
	union u_shnode *pattern;
	union u_shnode *body;
} t_nclist;

typedef struct s_ndefunc
{
	int type;
	int lin_no;
	char *text;
	union u_shnode *body;
} t_ndefunc;

typedef struct s_ndefun
{
	int type;
	union u_shnode *next;
	char *text;
	union u_shnode *body;
} t_ndefun;

typedef struct s_narg
{
	int type;
	union u_shnode *next;
	char *text;
	struct s_shnode_list *back_quote;
} t_narg;

typedef struct s_nfile
{
	int type;
	union u_shnode *next;
	int fd;
	union u_shnode *fname;
	char *expfname;
} t_nfile;

typedef struct s_ndup
{
	int type;
	union u_shnode *next;
	int fd;
	int dup_fd;
	union u_shnode *vname;
} t_ndup;

typedef struct s_nhere
{
	int type;
	union u_shnode *next;
	int fd;
	union u_shnode *doc;
} t_nhere;

typedef struct s_nnot
{
	int type;
	union u_shnode *com;
} t_nnot;

/* union/primary node type renamed to avoid collision */
typedef union u_shnode
{
	int type;
	t_ncmd ncmd;
	t_npipe npipe;
	t_nredir nredir;
	t_nbinary nbinary;
	t_nif nif;
	t_nfor nfor;
	t_ncase ncase;
	t_nclist nclist;
	t_ndefun ndefun;
	t_narg narg;
	t_nfile nfile;
	t_ndup ndup;
	t_nhere nhere;
	t_nnot nnot;
} t_shnode;

typedef struct s_shnode_list
{
	struct s_shnode_list *next;
	t_shnode *n;
} t_shnode_list;

typedef struct s_ftnode
{
	int count;
	t_shnode n;
} t_ftnode;

/* fix the tblentry 'next' pointer type so it points to same struct */
typedef struct s_tblentry
{
	struct s_tblentry *next;
	t_param param;
	short cmd_type;
	char rehash;
	char cmd_name[ARB];
} t_tblentry;

int builtinloc = -1;
t_tblentry *cmdtable[CMDTABLESIZE];

static void clear_cmd_entry()
{
	t_tblentry **tblp;
	t_tblentry **pp;
	t_tblentry *cmdp;

	intoff();
	tblp = cmdtable;
	while (tblp < &cmdtable[CMDTABLESIZE])
	{
		pp = tblp;
		cmdp = *pp;
		while (cmdp != NULL)
		{
			if (cmdp->cmd_type == CMDNORMAL || cmdp->cmd_type == CMDBUILTIN && cmdp->param.cmd->flags & BUILTIN_REGULAR && builtinloc > 0)
			{
				*pp = cmdp->next;
				xfree(cmdp);
			}
			else
				pp = &cmdp->next;
		}
		tblp++;
	}
	inton();
}

int changed;

void change_mail(const char *val)
{
	changed++;
}

/**
 * Fix command hash table when Path changed
 * called before PATH is changed. The argument is the new value of pathval()
 * still returns the old value at this point. called with interrupts off.
 */
static void change_path(const char *newval)
{
	const char *new;
	int idx;
	int bltin;

	new = newval;
	idx = 0;
	bltin = -1;
	while (1)
	{
		if (*new == '%' && prefix(new + 1, "builtin"))
		{
			bltin = idx;
			break;
		}
		new = ft_strchr(new, ':');
		if (!new)
			break;
		idx++;
		new++;
	}
	builtinloc = bltin;
	clear_cmd_entry();
}

t_var make_atty(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "ATTY\0", 0};
}

t_var make_ifs(const char *ifs)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, ifs, 0};
}

t_var make_mail(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "MAIL\0", change_mail};
}

t_var make_mailpath(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "MAILPATH\0", change_mail};
}

t_var make_path(const char *path)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, path, change_path};
}
