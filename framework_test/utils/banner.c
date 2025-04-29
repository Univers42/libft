#include "../framework.h"

/**
 * Prints a fancy header for the tester program
 */
void print_header(char *str)
{
    printf("\n");
    printf(CYAN BOLD "╔══════════════════════════════════════╗\n" RESET);
    printf(CYAN BOLD "║       LIBFT %-*s FUNCTION TESTER    ║\n" RESET,5, str);
    printf(CYAN BOLD "╚══════════════════════════════════════╝\n\n" RESET);
}
