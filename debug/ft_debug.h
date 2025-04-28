#ifndef DEBUG_H
# define DEBUG_H

# include "../ft_stddef.h"
void	ft_print_ascii(unsigned char *p, unsigned int size);
void	ft_print_hex(unsigned char *p, unsigned int size, unsigned long addr);
void	*ft_print_memory(void *addr, unsigned int size);
# endif