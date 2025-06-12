/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:22:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 22:24:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_debug.h"
#include "../lists/ft_list.h"
#include "../stdio/ft_stdio.h"

//void print_list(t_ps *)
//{
//    t_list *list;
//    int *value;
//
//    if (!ps || !ps->head_a)
//    {
//        ft_printf("Stack A: (empty)\n");
//        return ;
//    }
//    ft_printf("stack A: ");
//    list = ps->head_a;
//    while (list != NULL)
//    {
//        value = (int *)list->content;
//        if (value)
//        {
//            // Show: [value] at node_address -> next_node_address
//            ft_printf(GREEN"["MAGENTA"["BRIGHT_CYAN"[%d] "BLUE"at %p"MAGENTA"]"RESET, *value, (void*)list);
//            if (list->next)
//                ft_printf(RED" -> [next: %p]"GREEN"] \n"RESET, (void*)list->next);
//            else
//                ft_printf(RED" -> [next: NULL"GREEN"]"RESET);
//        }
//        else
//            ft_printf("[[NULL] at %p]", (void*)list);
//        
//        if (list->next)
//            ft_printf(" -> ");
//        list = list->next;
//    }
//    ft_printf("\n");
//}
//