/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:09:49 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/23 17:13:37 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "error.h"
#include <signal.h>
#include <unistd.h>


/**
 * return a string describing an error. then returned string may be a pointer
 * to a static buffer, that wil be overwritten on the next call.
 * Action describe the operation that got the error
 */

const char *err_msg(int e, int action)
{
    if (e != ENOENT && e != ENOTDIR)
        return str_error(e);
    if (action & E_OPEN)
        return ("No such file");
    else if(action & E_CREAT)
        return ("Directory nonexistent");
    else
        return ("not found");
}