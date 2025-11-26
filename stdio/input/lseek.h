/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lseek.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:09:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 22:54:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LSEEK_H
#define LSEEK_H

#include <unistd.h>
#include <sys/types.h>

off_t ft_lseek(int fd, off_t offset, int whence);

#endif