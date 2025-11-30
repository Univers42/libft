/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lseek.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:07:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/23 22:54:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lseek.h"
#include <unistd.h>
#include <errno.h>

/*
** ft_lseek - thin wrapper around the system lseek.
** Centralises any future platform-specific handling here.
*/
off_t ft_lseek(int fd, off_t offset, int whence)
{
    off_t ret;

    errno = 0;
    ret = lseek(fd, offset, whence);
    return (ret);
}