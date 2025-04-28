/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:44:49 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 22:19:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIST_H
# define FT_LIST_H
# include <stddef.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

// Add the prototype for ft_lstnew
t_list	*ft_lstnew(void *content);
t_list	*create_node(void *content);
t_list	*create_linked_list(int *arr, int n);
t_list	*deleteatposition(t_list *head, int pos);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
int		issorted(t_list *head);
int		kth_end_from_linked_list(t_list *head, int k);
t_list	*remove_duplicates(t_list *head);
int		search_content_lst(t_list *head, int x);
t_list	*sorted_merge(t_list *head1, t_list *head2);

#endif