/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dft_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 01:14:43 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 01:14:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "database.h"

// ============= DEFAULT CONFIGURATIONS =============

t_format_style	default_style(void)
{
    t_format_style	style;

    style.number_align = ALIGN_RIGHT;
    style.string_align = ALIGN_LEFT;
    /* Title: warm accent */
    style.title = make_color(220, 220, 100);      // warm yellow
    /* Header: visible blue for headers */
    style.header = make_color(144, 144, 144);      // DodgerBlue
    /* Footer: subtle gray */
    style.footer = make_color(10, 255, 150);     // medium gray
    /* Alternating rows: light subtle tint for even rows, white for odd rows */
    style.even_row = make_color(250, 250, 255);   // very light blue-ish / near-white
    style.odd_row = make_color(255, 255, 255);    // pure white
    return (style);
}

t_display_config default_config(void)
{
    t_display_config	cfg;

    cfg.show_title = true;
    cfg.show_header = true;
    cfg.show_footer = true;
    cfg.show_row_numbers = false;
    cfg.show_left_attributes = false;
    cfg.alternating_colors = true;
    cfg.use_bold_header = true;
    cfg.col_separator = ',';
    return (cfg);
}