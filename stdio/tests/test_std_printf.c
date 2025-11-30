// ...existing code...
#include "ft_stdlib.h"
#include "ft_memory.h"
#include "ft_stdio.h"
#include "ft_string.h"

enum e_fn {
    FT_PRINTF   = 1 << 0,
    FT_FPRINTF  = 1 << 1,
    FT_DPRINTF  = 1 << 2,
    FT_ASPRINTF = 1 << 3,
    FT_SPRINTF  = 1 << 4,
    FT_SNPRINTF = 1 << 5,
    FT_ALL      = (FT_PRINTF | FT_FPRINTF | FT_DPRINTF | FT_ASPRINTF | FT_SPRINTF | FT_SNPRINTF)
};

typedef struct s_fmts {
    const char *test_name;
    const char *fmt;
    int         flags;
} t_fmts;

static t_fmts lists_fmt[] = {
    {"multiple fmt only",  "%s %s %d", FT_ALL},
    {"single string",      "%s",       FT_ALL},
    {"integer only",       "%d",       FT_ALL},
    {NULL, NULL, 0}
};

void demo_printf(void)
{
    for (int i = 0; lists_fmt[i].fmt != NULL; ++i) {
        if (lists_fmt[i].flags & FT_PRINTF) {
            ft_printf("[FT_PRINTF] %s => ", lists_fmt[i].test_name);
            /* Example args for the three formats above */
            if (strcmp(lists_fmt[i].fmt, "%s %s %d") == 0)
                ft_printf(lists_fmt[i].fmt, "one", "two", 3);
            else if (strcmp(lists_fmt[i].fmt, "%s") == 0)
                ft_printf(lists_fmt[i].fmt, "single");
            else if (strcmp(lists_fmt[i].fmt, "%d") == 0)
                ft_printf(lists_fmt[i].fmt, 42);
            ft_printf("\n");
        }
    }
}

/* stubs for other demos you can fill similarly */
void demo_dprintf(void) { /* iterate lists_fmt and call ft_dprintf when flag set */ }
void demo_asprintf(void) { /* call ft_asprintf when flag set */ }
void demos_snprintf(void) { /* call ft_snprintf when flag set */ }
void demos_sprintf(void) { /* call ft_sprintf when flag set */ }

int main(void)
{
    demo_printf();
    return 0;
}
// ...existing code...