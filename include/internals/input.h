/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:59:16 by syzygy            #+#    #+#             */
/*   Updated: 2025/12/01 15:51:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
#define INPUT_H

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h> /* defines BUFSIZ */
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include "get_next_line.h"
#include "lseek.h"
#include "tee.h"

/* Ensure a reasonable maximum for multibyte characters used by the input code */
#ifndef PUNGETC_MAX
#ifdef MB_LEN_MAX
#define PUNGETC_MAX (MB_LEN_MAX > 16 ? MB_LEN_MAX : 16)
#else
#define PUNGETC_MAX 16
#endif
#endif

/* end-of-file and end-of-alias markers used by the input subsystem */
#ifndef PEOF
#define PEOF (-1)
#endif

#ifndef PEOA
#define PEOA (PEOF - 1)
#endif

typedef unsigned int tcflag_t;
typedef struct termios t_termios;


enum e_
{
	INPUT_PUSH_FILE = 1,
	INPUT_NOFILE_OK = 2
};

/* Forward-declare the alias struct (NO typedef) to avoid typedef conflicts.
   input.h only stores pointers to the alias node, so an incomplete
   struct declaration is sufficient here. The full typedef/definition
   remains in include/libalias.h. */
struct s_alias;

typedef struct s_block_str
{
	struct s_block_str *prev; // preceeding string on stack
	char *prev_string;
	int prevnleft;
	struct s_alias *ap;			 // if putsh was associated with an alias (use struct forward-decl)
	char *string;				 // remember the string since it may change
	struct s_block_str *sp_free; // delay freeing so we can stop nested aliases
	int unget;					 // number of outstanding calls to pungetc
} t_block_str;

typedef struct stdin_state
{
	tcflag_t canon;
	off_t seekable;
	t_termios tios;
	int pip[2];
	int pending;
} t_stdin_state;

/**
 * The parsefile structure pointed to by the global vairable parsefile contains information
 * about the current file beign read
 */
typedef struct s_parse_file
{
	struct s_parse_file *prev; // preceding file on stack
	int linno;				   // current line
	int fd;					   // file descriptor (or -1 is string)
	int nleft;				   // number of chars left in this line
	int eof;				   // do not read again once we hit EOF
	char *nextc;			   // next char in buffer
	char *buf;				   // input buffer
	t_block_str *strpush;	   // for pushing strings at this level
	t_block_str basestrpush;   // pushing one si fast
	t_block_str *sp_free;	   // delay freeing so we can stop nested aliases
	int lleft;				   // number of chars left in thsi buffer.
	int unget;				   // number of outstadnign calls to pungetc
} t_parse_file;

/* New: input-state singleton type that wraps previous globals */
typedef struct s_input
{
	t_parse_file basepf;	   /* top-level parsefile storage */
	t_parse_file *parsefile;   /* current parsefile pointer */
	t_stdin_state stdin_state; /* stdin state */
	int stdin_istty;		   /* -1 unknown, 0/1 */
} t_input;

/* Singleton accessor: returns pointer to the unique input state instance */
t_input *get_input(void);

/* Provide external (non-inline) versions of helpers if needed */
int input_get_lleft(t_parse_file *pf);
void input_set_lleft(t_parse_file *pf, int len);

#endif
