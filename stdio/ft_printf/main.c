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
    FILE *fp;

    printf("[TEST] ft_printf: Expect: Hello, world! Number: 42, Hex: 0x2a\n");
    ft_printf("[ft_printf] Hello, %s! Number: %d, Hex: %#x\n", "world", 42, 42);

    printf("\n[TEST] ft_sprintf: Expect: Pi approx: 3.14, Char: A\n");
    ret = ft_sprintf(buf, "[ft_sprintf] Pi approx: %.2f, Char: %c", 3.1415, 'A');
    printf("ft_sprintf returned: %d\nActual: %s\n", ret, buf);

    printf("\n[TEST] ft_snprintf: Expect: 21 + 21 = 42\n");
    ret = ft_snprintf(buf, sizeof(buf), "[ft_snprintf] %d + %d = %d", 21, 21, 42);
    printf("ft_snprintf returned: %d\nActual: %s\n", ret, buf);

    printf("\n[TEST] Standard printf: Expect: Hello, world! Number: 42, Hex: 0x2a\n");
    printf("[printf] Hello, %s! Number: %d, Hex: %#x\n", "world", 42, 42);

    printf("\n[TEST] Standard sprintf: Expect: Value: 12345\n");
    sprintf(buf, "Value: %d", 12345);
    printf("sprintf result: %s\n", buf);

    printf("\n[TEST] Standard snprintf (truncation): Expect: 'Hello' (5 chars + null)\n");
    snprintf(buf, 12, "HelloWorld"); // Only room for 5 chars + null
    printf("snprintf result (max 5 chars + null): '%s'\n", buf);

    // Compare ft_printf and printf
    printf("\n[TEST] Compare printf vs ft_printf\n");
    printf("[printf]  Expect: Hello, world! Number: 42, Hex: 0x2a\n");
    printf("[printf]  Actual: ");
    printf("Hello, %s! Number: %d, Hex: %#x\n", "world", 42, 42);
    printf("[ft_printf] Actual: ");
    ft_printf("Hello, %s! Number: %d, Hex: %#x\n", "world", 42, 42);

    // Compare sprintf and ft_sprintf
    printf("\n[TEST] Compare sprintf vs ft_sprintf\n");
    sprintf(buf, "Pi approx: %.2f, Char: %c", 3.1415, 'A');
    printf("[sprintf]   Expect/Actual: %s\n", buf);
    ft_sprintf(buf, "Pi approx: %.2f, Char: %c", 3.1415, 'A');
    printf("[ft_sprintf] Actual: %s\n", buf);

    // Compare snprintf and ft_snprintf (no truncation)
    printf("\n[TEST] Compare snprintf vs ft_snprintf (no truncation)\n");
    snprintf(buf, sizeof(buf), "%d + %d = %d", 21, 21, 42);
    printf("[snprintf]   Expect/Actual: %s\n", buf);
    ft_snprintf(buf, sizeof(buf), "%d + %d = %d", 21, 21, 42);
    printf("[ft_snprintf] Actual: %s\n", buf);

    // Compare snprintf and ft_snprintf (with truncation, safe)
    printf("\n[TEST] Compare snprintf vs ft_snprintf (truncation, safe)\n");
    snprintf(buf, 12, "HelloWorld");
    printf("[snprintf]   Expect/Actual: '%s'\n", buf);
    ft_snprintf(buf, 10, "HelloWorld");
    printf("[ft_snprintf] Actual: '%s'\n", buf);

    // Compare fprintf and ft_printf to file (if implemented)
    printf("\n[TEST] Standard fprintf to file: Expect: Output in test_output.txt\n");
    fp = fopen("test_output.txt", "w");
    if (fp)
    {
        fprintf(fp, "[fprintf] This line is written to a file.\n");
        fclose(fp);
        printf("Check 'test_output.txt' for output.\n");
    }
    else
    {
        printf("Failed to open file for writing.\n");
    }

    return 0;
}
