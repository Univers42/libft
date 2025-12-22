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

static t_format_style	default_style(void)
{
    t_format_style	style;

    style.number_align = ALIGN_RIGHT;
    style.string_align = ALIGN_LEFT;
    style.title = make_color(0, 255, 255);      // Cyan
    style.header = make_color(255, 255, 255);   // White
    style.footer = make_color(200, 200, 200);   // Light gray
    style.even_row = make_color(220, 220, 255); // Light blue
    style.odd_row = make_color(255, 255, 255);  // White
    return (style);
}

static t_display_config default_config(void)
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