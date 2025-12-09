/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_colors.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:10:27 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 18:40:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_COLORS_H
# define FT_COLORS_H

// Reset
# define RESET_TERM		"\033[0m"

// Regular colors
# define BLACK_TERM		"\033[0;30m"
# define GREY_TERM      "\033[0;30m]"
# define RED_TERM		"\033[0;31m"
# define GREEN_TERM		"\033[0;32m"
# define YELLOW_TERM	"\033[0;33m"
# define BLUE_TERM		"\033[0;34m"
# define MAGENTA_TERM	"\033[0;35m"
# define CYAN_TERM		"\033[0;36m"
# define WHITE_TERM		"\033[0;37m"

// Bright colors
# define BRIGHT_BLACK_TERM	"\033[1;30m"
# define BRIGHT_RED_TERM	"\033[1;31m"
# define BRIGHT_GREEN_TERM	"\033[1;32m"
# define BRIGHT_YELLOW_TERM	"\033[1;33m"
# define BRIGHT_BLUE_TERM	"\033[1;34m"
# define BRIGHT_MAGENTA_TERM	"\033[1;35m"
# define BRIGHT_CYAN_TERM	"\033[1;36m"
# define BRIGHT_WHITE_TERM	"\033[1;37m"

// Dim and bold
# define DIM_TERM		"\033[2m"
# define BOLD_TERM      "\033[1m"

/* ═══════════════════════════════════════════════════════════════════════════
   INFERNO THEME - Color Palette (Foreground Only - No Blocks)
   ═══════════════════════════════════════════════════════════════════════════ */

#define FG_FIRE "\001\033[38;5;196m\002"
#define FG_MOLTEN "\001\033[38;5;208m\002"
#define FG_EMBER "\001\033[38;5;214m\002"
#define FG_LAVA "\001\033[38;5;202m\002"
#define FG_ASH "\001\033[38;5;245m\002"
#define FG_CHARCOAL "\001\033[38;5;240m\002"
#define FG_DIM "\001\033[38;5;238m\002"
#define FG_SUCCESS "\001\033[38;5;82m\002"
#define FG_WARN "\001\033[38;5;220m\002"
#define FG_BLOOD "\001\033[38;5;124m\002"
#define FG_CYAN "\001\033[38;5;87m\002"
#define FG_MAGENTA "\001\033[38;5;213m\002"
#define FG_PURPLE "\001\033[38;5;135m\002"
#define FG_BLUE "\001\033[38;5;27m\002"
#define BOLD "\001\033[1m\002"
#define RESET "\001\033[0m\002"

/* ═══════════════════════════════════════════════════════════════════════════
   UNICODE SYMBOLS - Wide, Elegant Separators
   ═══════════════════════════════════════════════════════════════════════════ */

#define SEP_L " ═══ "
#define SEP_R " ═══ "
#define SEP_GIT " ║ "
#define SEP_THIN " ─── "
#define GIT_ICON ""
#define DIRTY "●"
#define CLEAN "○"
#define TIMER_ICON "⏱"
#define ARROW_OK " ●"
#define ARROW_FAIL "✗"
#define ARROW_WARN "⚠"
#define USER_ICON ""
#define DIR_ICON ""

#endif
