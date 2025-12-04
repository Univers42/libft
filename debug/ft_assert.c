/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_assert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:58:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 18:31:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Feature test macros - define before any system includes */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include <signal.h>
#include "ft_stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <execinfo.h>
#include <stdlib.h>
#include <string.h>

/* ANSI color codes for pretty output */
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

/* Minimal local replacement for Dl_info/dladdr */
typedef struct ft_DlInfo
{
	const char *dli_fname;
	void *dli_fbase;
	const char *dli_sname;
	void *dli_saddr;
} ft_DlInfo;

/* Very conservative stub: we do not attempt to resolve symbols.
 * Returns 0 and leaves fields NULL so callers can fallback.
 */
static int ft_dladdr(void *addr, ft_DlInfo *info)
{
	(void)addr;
	if (!info)
		return 0;
	info->dli_fname = NULL;
	info->dli_fbase = NULL;
	info->dli_sname = NULL;
	info->dli_saddr = NULL;
	return 0;
}

/* Parse a backtrace symbol line and extract useful info */
static void parse_and_print_frame(int idx, char *symbol, void *addr)
{
	char *start;
	char *end;
	char *offset_start;
	char func_name[256];
	char binary_name[256];
	char offset[64];

	/* Initialize buffers */
	func_name[0] = '\0';
	binary_name[0] = '\0';
	offset[0] = '\0';

	/* Format: binary(function+offset) [address] or binary(+offset) [address] */
	/* Extract binary name (before '(') */
	start = symbol;
	end = strchr(start, '(');
	if (end && (end - start) < 255)
	{
		strncpy(binary_name, start, end - start);
		binary_name[end - start] = '\0';
		start = end + 1;
	}

	/* Extract function name (between '(' and '+') */
	end = strchr(start, '+');
	if (end && (end - start) > 0 && (end - start) < 255)
	{
		strncpy(func_name, start, end - start);
		func_name[end - start] = '\0';
		offset_start = end + 1;
		/* Extract offset (between '+' and ')') */
		end = strchr(offset_start, ')');
		if (end && (end - offset_start) < 63)
		{
			strncpy(offset, offset_start, end - offset_start);
			offset[end - offset_start] = '\0';
		}
	}

	/* Print formatted frame with better visual hierarchy */
	ft_dprintf(STDERR_FILENO,
			   "  " BOLD CYAN "│" RESET " " DIM "#%02d" RESET " " BOLD "[%p]" RESET,
			   idx, addr);

	if (func_name[0] != '\0')
		ft_dprintf(STDERR_FILENO, " " BOLD GREEN "→ %s" RESET, func_name);
	else
		ft_dprintf(STDERR_FILENO, " " DIM YELLOW "→ <unknown>" RESET);

	if (offset[0] != '\0')
		ft_dprintf(STDERR_FILENO, " " DIM "+%s" RESET, offset);

	if (binary_name[0] != '\0')
		ft_dprintf(STDERR_FILENO, "\n      " BOLD CYAN "│" RESET " " DIM "in " BLUE "%s" RESET, binary_name);

	ft_dprintf(STDERR_FILENO, "\n");
}

/* Always print backtrace with enhanced formatting */
static void print_backtrace(void)
{
	void *buf[64];
	int n;
	int i;
	char **symbols;
	pid_t pid;

	n = backtrace(buf, (int)(sizeof(buf) / sizeof(buf[0])));
	pid = getpid();

	/* Header */
	ft_dprintf(STDERR_FILENO, "\n");
	ft_dprintf(STDERR_FILENO, BOLD RED "  ╔════════════════════════════════════════════════════════════╗\n" RESET);
	ft_dprintf(STDERR_FILENO, BOLD RED "  ║" RESET " " BOLD WHITE "🔍 STACK BACKTRACE" RESET " " DIM "(PID: %d, %d frames)" RESET "                " BOLD RED "║\n" RESET, pid, n);
	ft_dprintf(STDERR_FILENO, BOLD RED "  ╚════════════════════════════════════════════════════════════╝\n" RESET);
	ft_dprintf(STDERR_FILENO, "\n");

	/* Get human-readable symbols */
	symbols = backtrace_symbols(buf, n);

	/* Print each frame with parsing */
	for (i = 0; i < n; ++i)
	{
		if (symbols && symbols[i])
			parse_and_print_frame(i, symbols[i], buf[i]);
		else
			ft_dprintf(STDERR_FILENO, "  " BOLD CYAN "│" RESET " " DIM "#%02d" RESET " [%p]\n", i, buf[i]);
	}

	/* Footer */
	ft_dprintf(STDERR_FILENO, "\n");
	ft_dprintf(STDERR_FILENO, "  " BOLD CYAN "└─" RESET " " DIM "End of backtrace" RESET "\n");

	/* Free symbols (allocated by backtrace_symbols) */
	if (symbols)
		free(symbols);

	ft_dprintf(STDERR_FILENO, "\n");
}

void ft_abort(void)
{
	/* Prefer SIGABRT so previous output is visible; keep NORMA override */
	signal(SIGABRT, SIG_DFL);
	(void)kill(getpid(), SIGABRT);
}

static void xabort(void)
{
	ft_dprintf(STDERR_FILENO, "\n");
	ft_dprintf(STDERR_FILENO, BOLD RED "  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n" RESET);
	ft_dprintf(STDERR_FILENO, BOLD RED "  ┃" RESET " " BOLD WHITE "⚠️  ASSERTION FAILURE DETECTED" RESET "                          " BOLD RED "┃\n" RESET);
	ft_dprintf(STDERR_FILENO, BOLD RED "  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n" RESET);
	print_backtrace();
	ft_dprintf(STDERR_FILENO, BOLD RED "  ▸ " RESET BOLD "Process terminated via SIGABRT\n" RESET);
	ft_dprintf(STDERR_FILENO, "\n");
	ft_abort();
}

void ft_assert(int cond)
{
	void *caller;
	ft_DlInfo info;

	if (cond)
		return;

	/* Header banner */
	ft_dprintf(STDERR_FILENO, "\n");
	ft_dprintf(STDERR_FILENO, BOLD YELLOW "  ═══════════════════════════════════════════════════════════\n" RESET);
	ft_dprintf(STDERR_FILENO, BOLD RED "  ✗ ASSERTION FAILED" RESET "\n");
	ft_dprintf(STDERR_FILENO, BOLD YELLOW "  ───────────────────────────────────────────────────────────\n" RESET);

	caller = __builtin_return_address(0);
	if (ft_dladdr(caller, &info) && info.dli_sname)
		ft_dprintf(STDERR_FILENO,
				   "  " DIM "Function:  " RESET GREEN "%s" RESET "\n"
				   "  " DIM "Object:    " RESET BLUE "%s" RESET "\n",
				   (info.dli_sname ? info.dli_sname : "??"),
				   (info.dli_fname ? info.dli_fname : "??"));
	else
		ft_dprintf(STDERR_FILENO,
				   "  " DIM "Caller:    " RESET BOLD "%p" RESET "\n", caller);

	ft_dprintf(STDERR_FILENO, BOLD YELLOW "  ═══════════════════════════════════════════════════════════\n" RESET);

	xabort();
}
