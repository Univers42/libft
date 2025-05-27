/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_render.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:45:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 19:01:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RENDER_H
# define FT_RENDER_H
# include "ft_stddef.h"
# include "ft_colors.h"

/* Banner styles */
typedef enum e_banner_style
{
	BANNER_SIMPLE,
	BANNER_DOUBLE,
	BANNER_ROUNDED,
	BANNER_THICK,
	BANNER_ASCII_ART
}	t_banner_style;

/* Box drawing characters */
# define BOX_SIMPLE_H "─"
# define BOX_SIMPLE_V "│"
# define BOX_SIMPLE_TL "┌"
# define BOX_SIMPLE_TR "┐"
# define BOX_SIMPLE_BL "└"
# define BOX_SIMPLE_BR "┘"

# define BOX_DOUBLE_H "═"
# define BOX_DOUBLE_V "║"
# define BOX_DOUBLE_TL "╔"
# define BOX_DOUBLE_TR "╗"
# define BOX_DOUBLE_BL "╚"
# define BOX_DOUBLE_BR "╝"

/* Core rendering functions */
void	ft_print_banner(const char *title, const char *subtitle, t_banner_style style);
void	ft_print_box(const char *content, int width, t_banner_style style);
void	ft_print_separator(int width, const char *char_set);
void	ft_print_progress_bar(int current, int total, int width);
void	ft_print_header(const char *project_name, const char *version);
void	ft_print_footer(void);
void	ft_print_ascii_art(const char *text);
void	ft_print_centered_text(const char *text, int width);
void	ft_print_status(const char *message, const char *status, const char *color);
void	ft_clear_screen(void);
void	ft_move_cursor(int x, int y);

/* Advanced rendering functions */
void	ft_print_table(char **headers, char ***data, int rows, int cols);
void	ft_print_tree(const char *root, char **branches, int depth);
void	ft_print_loading_spinner(int duration_ms);
void	ft_print_menu(char **options, int count, int selected);
void	ft_print_graph(int *values, int count, int max_height);
void	ft_print_logo_ascii(const char *text);
void	ft_print_notification(const char *title, const char *message, const char *type);
void	ft_print_code_block(const char *code, const char *language);
void	ft_print_diff(const char *old_line, const char *new_line);
void	ft_print_terminal_frame(const char *title, const char *content, int width, int height);
void	ft_print_emoji_status(const char *message, const char *emoji);
void	ft_print_columns(char **col1, char **col2, int rows);
void	ft_print_timeline(char **events, char **times, int count);

/* Animation and interactive functions */
void	ft_typewriter_effect(const char *text, int delay_ms);
void	ft_fade_in_text(const char *text);
void	ft_print_slide_animation(const char **slides, int count);

/* Utility functions */
int		ft_get_terminal_width(void);
int		ft_get_terminal_height(void);
void	ft_print_padding(int count, char c);
void	ft_print_responsive_banner(const char *title, const char *subtitle);
void	ft_print_adaptive_table(char **data, int rows, int cols);

#endif
