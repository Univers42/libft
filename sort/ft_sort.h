/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:18:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 13:47:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SORT_H
#define SORT_H

int     *ft_quick_sort(int *arr, int low, int high);
void    ft_chunk_sort(int *arr, int size);
void	ft_merge_sort(int *arr, int l, int r);
void	merge(int *arr, int l, int m, int r);
#endif