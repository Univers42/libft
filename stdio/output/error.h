/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:05:18 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/23 17:09:23 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H
# define E_OPEN 01
# define E_CREAT 02
# define E_EXEC 04
# define EXINT 0
# define EXERR 1
# define EXEND 3
# define EXEXIT 4

void    ft_error(const char *, ...);
void    exerror(int, const char *, ...);
const char  *errmsg(int, int);
void    ft_warn(const char *, ...);
#endif