#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "writer.h"

// Prototypes for the printf family
int ft_printf(const char *format, ...);
int ft_sprintf(char *dst, const char *format, ...);
int ft_snprintf(char *dst, size_t cap, const char *format, ...);

int main(void)
{
    char buf[128];
    int ret;

    ft_printf("[ft_printf] Hello, %s! Number: %d, Hex: %#x\n", "world", 42, 42);

    ret = ft_sprintf(buf, "[ft_sprintf] Pi approx: %.2f, Char: %c", 3.1415, 'A');
    printf("ft_sprintf returned: %d, buf: %s\n", ret, buf);

    ret = ft_snprintf(buf, sizeof(buf), "[ft_snprintf] %d + %d = %d", 21, 21, 42);
    printf("ft_snprintf returned: %d, buf: %s\n", ret, buf);

    // Compare with standard printf
    printf("[printf] Hello, %s! Number: %d, Hex: %#x\n", "world", 42, 42);

    return 0;
}
