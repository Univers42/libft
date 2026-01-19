/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:22:22 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/10 22:47:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/* Feature test macros - define before any system includes */
# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

# ifndef _DEFAULT_SOURCE
#  define _DEFAULT_SOURCE
# endif

# ifndef _BSD_SOURCE
#  define _BSD_SOURCE
# endif

//# include "analyzer.h"
# include "ds.h"
# include "ft_colors.h"
# include "ft_ctype.h"
# include "ft_debug.h"
# include "ft_math.h"
# include "ft_memory.h"
# include "ft_render.h"
# include "ft_sort.h"
# include "ft_stddef.h"
# include "ft_stdio.h"
# include "ft_stdlib.h"
# include "ft_string.h"
# include "termcap.h"
# include "ft_time.h"
# include "ft_wctype.h"
//# include "get.h" //!PROTOTYPE NOT WORKING
# include "ipc.h"
# include "system.h"

# include <immintrin.h>
# include <unistd.h>
# include <signal.h>
# include <time.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/stat.h>
# include <dirent.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <signal.h>

# ifdef TESTER
#  include "tester.hpp"
# endif

char	*ft_ltoa(long n);
long	ft_atol(const char *str);

#endif
