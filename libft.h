/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dyl-syzygy <dyl-syzygy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:38:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/03/17 13:42:23 by dyl-syzygy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
//write(), malloc()
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>

typedef long				t_ssize;
typedef unsigned long int	t_size;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

// ==============   <CTYPE> LIBRARY		============== 7

int		ft_isdigit(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
char	*ft_itoa(int n);
char	*ft_utoa(unsigned int n);
char	*ft_xtoa(unsigned long int num, int is_upper);
int		ft_atoi(const char *nptr);
void	ft_putnbr_base(int nbr, char *radix);
int		ft_isspace(int c);
int		ft_atoi_base(const char *str, int base);

// =============== MATH function ======================
//int		ft_abs(int n);
//int		ft_max(int a, int b);
//int		ft_min(int a, int b);
int		ft_log10(int n);

// ==============	MEMORY MANIPULATION	============== 7

void	ft_bzero(void *s, t_size n);
void	*ft_memset(void *ptr, int value, t_size num);
void	*ft_memcpy(void *dest, const void *src, t_size n);
void	*ft_memmove(void *dest, const void *src, t_size n);
void	*ft_memchr(const void *s, int c, t_size n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_print_memory(void *addr, unsigned int size);
void	ft_print_hex(unsigned char *p, unsigned int size, unsigned long addr);
void	ft_print_ascii(unsigned char *p, unsigned int size);
// ==============	<STDIO> library		============== 7

int		ft_tolower(int c);
int		ft_toupper(int c);

/*______File Descriptor Function_______*/

void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

// ==============	String function	============== 14

t_size	ft_strlen(const char *s);
t_size	ft_strlcpy(char *dst, const char *src, t_size dsize);
t_size	ft_strlcat(char *dst, const char *src, t_size dsize);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_substr(const char *s, unsigned int start, t_size len);
char	*ft_strtrim(const char *s1, const char *set);
char	**ft_split(const char *s, char c);
char	*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
int		ft_strncmp(const char *s1, const char *s2, t_size n);
char	*ft_strnstr(const char *big, const char *little, t_size len);
char	*ft_strdup(const char *s);

//==============	Bonus linked list prototype 	============== 9

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
