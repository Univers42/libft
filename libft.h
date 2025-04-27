/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:38:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 18:56:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
//write(), malloc()
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>

#include "utils_lib/ft_typedef.h"
#include "strings/ft_string.h"
#include "memory/ft_memory.h"
#include "ctype/ft_ctype.h"
#include "lists/ft_list.h"
#include "math/ft_math.h"
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

// ==============   <CTYPE> LIBRARY		============== 7


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
void	ft_first_word(char *str);

/*______File Descriptor Function_______*/

void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

// ==============	String function	============== 14



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

// ==============	IN-place memory algorithm	==============
void	ft_swap(void *a, void *b, t_size size);
void	ft_reverse(void *arr, t_size size, t_size n);
// ===================	<FT_PRINTF> function	==============
int		ft_printf(const char *format, ...);
//=====================	<GET_NEXT_LINE> function	==============
int		get_next_line(int fd, char **line);
#endif
