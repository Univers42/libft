#ifndef FT_MEMORY_H
# define FT_MEMORY_H
# include "../ft_stddef.h"

int		ft_tolower(int c);
int		ft_toupper(int c);
void	ft_swap(void *a, void *b, t_size size);
void	ft_reverse(void *arr, t_size size, t_size n);
void	ft_bzero(void *s, t_size n);
void	*ft_memset(void *ptr, int value, t_size num);
void	*ft_memcpy(void *dest, const void *src, t_size n);
void	*ft_memmove(void *dest, const void *src, t_size n);
void	*ft_memchr(const void *s, int c, t_size n);
int		ft_memcmp(const void *s1, const void *s2, t_size n);
void	*ft_print_memory(void *addr, unsigned int size);
void	ft_print_hex(unsigned char *p, unsigned int size, unsigned long addr);
void	ft_print_ascii(unsigned char *p, unsigned int size);

#endif