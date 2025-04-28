#ifndef FT_STDIO_H
#define FT_STDIO_H

void	ft_first_word(char *str);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putnbr_base(int nbr, char *radix);
int		ft_printf(const char *format, ...);
int		get_next_line(int fd, char **line);

#endif