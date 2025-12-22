/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_csv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 01:08:01 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 01:08:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "database.h"

// Proper CSV split function that handles quoted fields and escaped quotes ("")
static char **ft_split(const char *str, char delimiter) {
    char **result = NULL;
    size_t count = 0;
    const char *p = str;
    bool in_quotes = false;

    if (!str)
        return NULL;
    while (*p) {
        if (*p == '"') {
            in_quotes = true;
            p++;
            while (*p) {
                if (*p == '"' && p[1] == '"') {
                    p += 2;
                    continue;
                }
                if (*p == '"') {
                    p++;
                    in_quotes = false;
                    break;
                }
                p++;
            }
        } else if (*p == delimiter && !in_quotes) {
            count++;
            p++;
        } else {
            p++;
        }
    }
    count++;

    result = calloc(count + 1, sizeof(char *));
    if (!result)
        return NULL;
    p = str;
    size_t idx = 0;
    while (idx < count) {
        bool was_quoted = false;
        size_t write_pos = 0;
        size_t buf_cap = strlen(p) + 1;
        char *field = calloc(buf_cap + 1, 1);
        if (!field) {
            free_split(result);
            return NULL;
        }

        if (*p == '"') {
            was_quoted = true;
            p++;
            while (*p) {
                if (*p == '"' && p[1] == '"') {
                    field[write_pos++] = '"';
                    p += 2;
                    continue;
                }
                if (*p == '"') {
                    p++;
                    break;
                }
                field[write_pos++] = *p++;
            }
            while (*p && *p != delimiter && (*p == ' ' || *p == '\t'))
                p++;
            if (*p == delimiter)
                p++;
        } else {
            while (*p && *p != delimiter) {
                field[write_pos++] = *p++;
            }
            if (*p == delimiter)
                p++;
        }
        field[write_pos] = '\0';

        if (!was_quoted) {
            char *trimmed = trim_whitespace(field);
            if (trimmed != field) {
                char *dup = ft_strdup(trimmed);
                free(field);
                field = dup ? dup : ft_strdup("");
            }
        }

        result[idx++] = field;
    }
    result[idx] = NULL;
    return result;
}

static void free_split(char **arr) {
    size_t i;
    if (!arr) return;
    for (i = 0; arr[i]; i++) {
        free(arr[i]);
    }
    free(arr);
}

static char *trim_whitespace(char *str) {
    char *end;
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;
    *(end + 1) = 0;
    return str;
}
