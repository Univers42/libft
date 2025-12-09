#ifdef __cplusplus
/* avoid C++ keyword 'new' in header by mapping it to new_ just while including */
#define new new_
extern "C"
{
#endif
#include "ds.h"
#ifdef __cplusplus
}
#undef new
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/* ANSI color macros */
#define C_RESET "\x1b[0m"
#define C_BOLD "\x1b[1m"
#define C_RED "\x1b[31m"
#define C_GREEN "\x1b[32m"
#define C_YELLOW "\x1b[33m"
#define C_BLUE "\x1b[34m"
#define C_MAG "\x1b[35m"
#define C_CYAN "\x1b[36m"

static void title(const char *fmt)
{
    printf("%s%s%s\n", C_BOLD C_CYAN, fmt ? fmt : "", C_RESET);
}
static void subtitle(const char *fmt)
{
    printf("%s%s%s\n", C_BOLD C_YELLOW, fmt ? fmt : "", C_RESET);
}
static void print_meta(t_arr *a, const char *when)
{
    printf("%s  META %s:%s nelem=%zu max_idx=%zu first_idx=%zu alloc_size=%zu\n",
           C_MAG, when ? when : "", C_RESET,
           array_num_elements(a), array_max_index(a),
           array_first_index(a), array_alloc_size(a));
}

/* Tokenizer: handles quotes and backslashes, returns array of strdup'd tokens */
static t_arr *tokenize_string(const char *s)
{
    t_arr *a = array_create();
    size_t len = s ? strlen(s) : 0;
    char *buf = (char *)malloc(len + 1);
    size_t bi = 0;
    bool in_single = false, in_double = false;
    size_t i;
    if (!a || !buf)
        return NULL;
    for (i = 0; i <= len; ++i)
    {
        char c = s[i];
        if (c == '\\' && i < len)
        {
            buf[bi++] = s[++i];
            continue;
        }
        if (!in_single && c == '"')
        {
            in_double = !in_double;
            continue;
        }
        if (!in_double && c == '\'')
        {
            in_single = !in_single;
            continue;
        }
        if (!in_single && !in_double && (c == '\0' || isspace((unsigned char)c)))
        {
            if (bi > 0)
            {
                buf[bi] = '\0';
                array_push(a, strdup(buf));
                bi = 0;
            }
            continue;
        }
        if (c != '\0')
            buf[bi++] = c;
    }
    free(buf);
    return a;
}

/* Classify token for printing */
static const char *token_type(const char *t)
{
    if (!t)
        return "null";
    if ((strcmp(t, "&&") == 0) || (strcmp(t, "||") == 0) || (strcmp(t, ">>") == 0))
        return "OP";
    if (strlen(t) == 1 && strchr("|&;<>", t[0]))
        return "OP";
    if (strchr(t, ' ') || strchr(t, '\t'))
        return "QUOTED";
    return "WORD";
}

/* Print tokens with colors */
static void print_tokens(t_arr *tokens)
{
    t_arr_elem *cur;
    if (!tokens)
        return;
    subtitle(" Tokens:");
    for (cur = element_forw(tokens->head); cur != tokens->head; cur = element_forw(cur))
    {
        char *v = element_value(cur);
        const char *typ = token_type(v);
        if (strcmp(typ, "OP") == 0)
            printf("  %sOP%s [%zu] %s%s%s\n", C_RED, C_RESET, element_index(cur), C_RED, v, C_RESET);
        else if (strcmp(typ, "QUOTED") == 0)
            printf("  %sQUOTED%s [%zu] %s%s%s\n", C_YELLOW, C_RESET, element_index(cur), C_YELLOW, v, C_RESET);
        else
            printf("  %sWORD%s [%zu] %s%s%s\n", C_GREEN, C_RESET, element_index(cur), C_GREEN, v, C_RESET);
    }
}

/* Parse a simple pipeline: slice tokens between '|' and print commands */
static void parse_and_print_pipeline(t_arr *tokens)
{
    t_arr_elem *start = element_forw(tokens->head);
    t_arr_elem *cur;
    int cmdno = 1;

    for (cur = start; cur != tokens->head; cur = element_forw(cur))
    {
        char *v = element_value(cur);
        if (v && strcmp(v, "|") == 0)
        {
            t_arr *cmd = array_slice(tokens, start, cur);
            printf("%sCommand %d:%s\n", C_BOLD C_BLUE, cmdno++, C_RESET);
            subtitle("  Arguments:");
            for (t_arr_elem *e = element_forw(cmd->head); e != cmd->head; e = element_forw(e))
                printf("    %s%s%s\n", C_CYAN, element_value(e), C_RESET);
            array_dispose(cmd);
            start = element_forw(cur);
        }
    }
    if (start && start != tokens->head)
    {
        t_arr *cmd = array_slice(tokens, start, tokens->head);
        printf("%sCommand %d:%s\n", C_BOLD C_BLUE, cmdno++, C_RESET);
        subtitle("  Arguments:");
        for (t_arr_elem *e = element_forw(cmd->head); e != cmd->head; e = element_forw(e))
            printf("    %s%s%s\n", C_CYAN, element_value(e), C_RESET);
        array_dispose(cmd);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        title("Array demo with colorized tokenization/parse");
        printf(" Usage: %s <command parts...>\n", argv[0]);
        printf(" Example: %s echo \"hello world\" | wc -w\n", argv[0]);
        return 0;
    }

    /* Join argv into a single input string */
    size_t tot = 0;
    for (int i = 1; i < argc; ++i)
        tot += strlen(argv[i]) + 1;
    char *input = (char *)malloc(tot + 1);
    if (!input)
        return 1;
    input[0] = '\0';
    for (int i = 1; i < argc; ++i)
    {
        strcat(input, argv[i]);
        if (i + 1 < argc)
            strcat(input, " ");
    }

    title("Step 1: Tokenize input");
    printf(" Input: %s%s%s\n\n", C_BOLD, input, C_RESET);

    t_arr *tokens = tokenize_string(input);
    print_meta(tokens, "after tokenize");
    print_tokens(tokens);
    printf("\n");

    title("Step 2: Simple parse into pipeline commands (split on '|')");
    parse_and_print_pipeline(tokens);
    printf("\n");

    title("Step 3: Demonstrate array operations on tokens");
    subtitle(" Remove token index 0 (if exists) with array_remove");
    if (!array_empty(tokens))
    {
        t_arr_elem *removed = array_remove(tokens, 0);
        if (removed)
        {
            printf("  Removed: %s%s%s\n", C_RED, element_value(removed), C_RESET);
            array_dispose_element(removed);
        }
        print_meta(tokens, "after remove(0)");
        print_tokens(tokens);
    }
    printf("\n");

    subtitle(" Push a new token 'NEW' at front and then pop it");
    array_push(tokens, strdup("NEW"));
    print_meta(tokens, "after push(NEW)");
    print_tokens(tokens);
    array_pop(tokens);
    print_meta(tokens, "after pop()");
    print_tokens(tokens);
    printf("\n");

    subtitle(" Make a slice of first two tokens (if enough) and copy it");
    {
        t_arr_elem *s = element_forw(tokens->head);
        t_arr_elem *e = s;
        int k;
        for (k = 0; k < 2 && e != tokens->head; ++k)
            e = element_forw(e);
        if (s != tokens->head)
        {
            t_arr *slice = array_slice(tokens, s, e);
            t_arr *copy = array_copy(slice);
            print_meta(slice, "slice meta");
            print_tokens(slice);
            print_meta(copy, "copy meta");
            print_tokens(copy);
            array_dispose(slice);
            array_dispose(copy);
        }
    }
    printf("\n");

    title("Cleanup");
    array_dispose(tokens);
    free(input);

    printf("%sDemo complete.%s\n", C_GREEN, C_RESET);
    return 0;
}
