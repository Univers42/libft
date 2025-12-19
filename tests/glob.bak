/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 19:07:54 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 19:07:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>

extern "C"
{
#include "../include/internals/lexer.h"
#include "../include/glob.h"
#include "../include/internals/ft_vector.h"
}

int main()
{
    std::string line;
    std::cout << "Interactive glob demo. Type 'exit' to quit.\n";
    while (true)
    {
        std::cout << "> " << std::flush;
        if (!std::getline(std::cin, line))
            break;
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
            line.pop_back();
        if (line == "exit")
            break;
        if (line.empty())
            continue;

        t_ast_node word;
        memset(&word, 0, sizeof(word));
        word.node_type = AST_WORD;
        t_vec_config cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.elem_size = sizeof(t_ast_node);
        cfg.initial_capacity = 4;
        cfg.type_mask = VEC_TYPE_PLAIN;
        cfg.copy_fn = NULL;
        cfg.free_fn = NULL;
        if (vec_init(&word.children, &cfg) != 0)
        {
            std::cerr << "vec_init failed\n";
            return 1;
        }

        t_token *tok = (t_token *)malloc(sizeof(t_token));
        if (!tok)
        {
            std::cerr << "malloc failed\n";
            vec_destroy(&word.children);
            return 1;
        }
        char *lexeme = strdup(line.c_str());
        if (!lexeme)
        {
            free(tok);
            vec_destroy(&word.children);
            std::cerr << "strdup failed\n";
            return 1;
        }
        tok->type = TOKEN_WORD;
        tok->start = lexeme;
        tok->len = (int)line.size();
        tok->line = 1;
        tok->allocated = false;

        t_ast_node child;
        memset(&child, 0, sizeof(child));
        child.node_type = AST_TOKEN;
        child.token = tok;
        if (!vec_push(&word.children, &child))
        {
            std::cerr << "vec_push failed\n";
            free(lexeme);
            free(tok);
            vec_destroy(&word.children);
            return 1;
        }

        t_dyn_str repr = word_to_string(word);
        std::cout << "Reconstructed word: " << (repr.buff ? repr.buff : "") << "\n";
        free(repr.buff);

        t_vec result = expand_word_glob(&word);

        std::cout << "Matches (" << (int)result.len << "):\n";
        for (size_t i = 0; i < result.len; ++i)
        {
            char **slot = (char **)vec_idx(&result, i);
            if (slot && *slot)
                std::cout << "  - " << *slot << "\n";
        }

        vec_destroy(&result);

        free(lexeme);
        free(tok);
        vec_destroy(&word.children);
    }
    std::cout << "Goodbye.\n";
    return 0;
}
