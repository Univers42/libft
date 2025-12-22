/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_table2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 01:11:43 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 01:11:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "database.h"

void calculate_column_widths(t_database *db, size_t *widths, size_t *label_width)
{
    size_t i, j;
    int len;

    *label_width = 0;

    // Calculate label width if needed
    if (db->config.show_left_attributes) {
        for (i = 0; i < db->nrows; i++) {
            if (db->rows[i].label) {
                len = display_width(db->rows[i].label);
                *label_width = max_size(*label_width, (size_t)len);
            }
        }
        *label_width = max_size(*label_width, 8); // Minimum width
    }

    // Initialize with column name display lengths (headers)
    for (i = 0; i < db->ncols; i++) {
        widths[i] = display_width(db->cols[i].name);
    }

    // Find maximum display width in each column
    for (i = 0; i < db->nrows; i++) {
        for (j = 0; j < db->ncols && j < db->rows[i].ncols; j++) {
            if (db->rows[i].data[j]) {
                len = display_width(db->rows[i].data[j]);
                widths[j] = max_size(widths[j], (size_t)len);
            }
        }
    }

    // Apply fixed widths if specified (override calculated widths)
    for (i = 0; i < db->ncols; i++) {
        if (db->cols[i].width > 0) {
            widths[i] = db->cols[i].width;
        }
    }
}

// return displayed column width (wcwidth-aware) of a UTF-8 string
int display_width(const char *s)
{
    mbstate_t st;
    ft_memset(&st, 0, sizeof(st));
    const unsigned char *p = (const unsigned char *)s;
    size_t bytes;
    wchar_t wc;
    int w = 0;
    while (*p)
    {
        bytes = ft_mbrtowc(&wc, (const char *)p, ft_mb_cur_max(), &st);
        if (bytes == (size_t)-1 || bytes == (size_t)-2 || bytes == 0)
        {
            // invalid multibyte -> count as single column and advance one byte
            w += 1;
            p++;
            ft_memset(&st, 0, sizeof(st));
            continue;
        }
        int cw = ft_wcwidth(wc);
        if (cw < 0) cw = 0;
        w += cw;
        p += bytes;
    }
    return w;
}
