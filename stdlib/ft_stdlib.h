#ifndef FT_STDLIB_H
# define FT_STDLIB_H
# include "../ft_stddef.h"

char	*ft_itoa(int n);
char	*ft_utoa(unsigned int n);
char	*ft_xtoa(unsigned long int num, int is_upper);
int		ft_atoi(const char *nptr);
void	ft_putnbr_base(int nbr, char *radix);
int		ft_atoi_base(const char *str, int base);
void	*ft_calloc(t_size nmemb, t_size size);
#endif