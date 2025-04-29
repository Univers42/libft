#ifndef FRAMEWORK_H
# define FRAMEWORK_H
# include "/home/dlesieur/Goinfre/minitalk/libft/libft.h"
# include <stdio.h>
# include <ctype.h>
# include <time.h>
# include <string.h>
# include <unistd.h>
/* Test parameters */
#define BENCHMARK_ITERATIONS 10000000
#define CHAR_RANGE 256  /* Test all possible char values including extended ASCII */

/* Function pointer type for ctype-style functions */
typedef int (*t_ctype_func)(int);

//UTILS
void print_header(char *str);
#endif