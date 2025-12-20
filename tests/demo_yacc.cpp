/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_yacc.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 04:30:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 02:02:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <cstring>

extern "C" {
#include "ft_yacc.h"
#include "lexer.h"
#include "ft_string.h"
#include "ft_queue.h"
}

int main()
{
    t_yopt opts;
    std::memset(&opts, 0, sizeof(opts));
    /* skip whitespace between tokens */
    opts.skip_whitespace = 1;
    t_yacc *y = yacc_create(opts);
    if (!y)
    {
        std::cerr << "Failed to create yacc parser\n";
        return 1;
    }
    const char *src =
        "#!/bin/bash\n"
        "function compute() {\n"
        "  local sum=0\n"
        "  for i in 1 2 3 4; do\n"
        "    sum $((sum + i))\n"
        "  done\n"
        "  if [ $sum -gt 5 ]; then\n"
        "    echo \"big\"\n"
        "    return (0)\n"
        "  else\n"
        "    echo \"small\"\n"
        "    return (1)\n"
        "  fi\n"
        "}\n"
		"((cat - e \"this is a subshell\"))"
        "\n"
        "# call compute with fallback and pipe its output\n"
        "compute ${ARGS:-default} | grep big && echo \"done\" # end\n";
    y->input.buff = ft_strdup(src);
    if (!y->input.buff)
    {
        std::cerr << "Failed to allocate input buffer\n";
        yacc_destroy(y);
        return 1;
    }
    y->input.len = (size_t)ft_strlen(y->input.buff);
    y->input.cap = y->input.len + 1;
    y->input.pos = 0;
    /* zero-init position struct to silence -Wmissing-field-initializers */
    std::memset(&y->pos, 0, sizeof(y->pos));

    yacc_setup_deft_lexer(y);

    t_deque tokens;
    if (deque_init(&tokens, 64, sizeof(t_token), NULL))
    {
        std::cerr << "Failed to init token deque\n";
        yacc_destroy(y);
        return 1;
    }

    /* Iterate using yacc_next_token and convert to t_token for debug printing */
    while (y->input.pos < y->input.len)
    {
        /* compute start after skipping whitespace to avoid leading spaces in lexeme */
        size_t start = y->input.pos;
        while (start < y->input.len && ft_isspace((int)dyn_str_idx(&y->input, start)))
            start++;
        int prev_line = y->pos.line;
        t_ytoken *yt = yacc_next_token(y);
        if (!yt)
            break;
        size_t after = y->input.pos;
        int len = (int)(after - start);
        t_token tk;
        tk.type = (t_token_type)yt->type;
        tk.start = y->input.buff + start;
        tk.len = len;
        tk.line = prev_line;
        tk.allocated = false;
        deque_push_end(&tokens, &tk);
        free(yt);
    }

    print_tokens(&tokens);
    print_token_summary(&tokens);

    /* manual destroy: free internal buffer and clear struct (deque_destroy symbol may not be exposed) */
    if (tokens.buf)
        free(tokens.buf);
    std::memset(&tokens, 0, sizeof(tokens));
     /* y->input was malloc'd by ft_strdup; dyn_str_free will free it */
     yacc_destroy(y);
     return 0;
}

