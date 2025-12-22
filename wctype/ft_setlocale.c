/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setlocale.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 02:23:29 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 02:23:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setlocale.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wctype.h"
#include <string.h>
#include <stdlib.h>

/* internal storage size */
enum { LOCSIZE = 64 };

/* file-scope locale storage so cleanup can access it */
static char	locales[FT_CAT_COUNT][LOCSIZE];
static int	initialized = 0;
/* track max UTF-8 bytes for callers (replacement for MB_CUR_MAX) */
static size_t	g_mb_cur_max = 4; /* default to UTF-8 safe width */

/* Pure internal ft_setlocale implementation (never calls system setlocale). */
char *ft_setlocale(int category, const char *locale)
{
	size_t i;

	if (!initialized)
	{
		for (i = 0; i < (size_t)FT_CAT_COUNT; ++i)
			locales[i][0] = '\0';
		/* default C locale for ctype category */
		strncpy(locales[FT_LC_CTYPE < FT_CAT_COUNT ? FT_LC_CTYPE : 0], "C", LOCSIZE - 1);
		locales[FT_LC_CTYPE < FT_CAT_COUNT ? FT_LC_CTYPE : 0][LOCSIZE - 1] = '\0';
		initialized = 1;
	}

	if (category < 0 || category >= FT_CAT_COUNT)
		return NULL;

	/* Query current locale */
	if (locale == NULL)
		return (locales[category][0] ? locales[category] : NULL);

	/* Empty string -> lookup environment variables */
	if (locale[0] == '\0')
	{
		const char *env = getenv("LC_ALL");
		if (!env || !*env) env = getenv("LC_CTYPE");
		if (!env || !*env) env = getenv("LANG");
		if (!env) env = "C";

		strncpy(locales[category], env, LOCSIZE - 1);
		locales[category][LOCSIZE - 1] = '\0';

		/* Check for UTF-8 in the environment string to set width */
		if (strstr(env, "UTF-8") || strstr(env, "utf8") || strstr(env, "UTF8"))
			g_mb_cur_max = 4;
		else
			g_mb_cur_max = 1;

		return locales[category];
	}

	/* Store provided locale string (truncate if needed) */
	strncpy(locales[category], locale, LOCSIZE - 1);
	locales[category][LOCSIZE - 1] = '\0';

	/* Decide multibyte width: UTF-8 or not */
	if (strstr(locale, "UTF-8") || strstr(locale, "utf8") || strstr(locale, "UTF8"))
		g_mb_cur_max = 4;
	else
		g_mb_cur_max = 1;

	return locales[category];
}

size_t	ft_mb_cur_max(void)
{
	return g_mb_cur_max;
}

void ft_locale_cleanup(void)
{
	size_t i;
	for (i = 0; i < (size_t)FT_CAT_COUNT; ++i)
		locales[i][0] = '\0';
	initialized = 0;
	g_mb_cur_max = 4;
}

/* ************************************************************************** */
/*                              TEST MAIN                                     */
/* ************************************************************************** */

//#include <stdio.h>
//
//int	main(void)
//{
//	char	*loc;
//
//	printf("=== ft_setlocale Test (Norminette Compliant) ===\n\n");
//	loc = ft_setlocale(FT_LC_ALL, NULL);
//	printf("Current locale: %s\n", loc ? loc : "(null)");
//	loc = ft_setlocale(FT_LC_ALL, "C");
//	printf("After setting to 'C': %s\n", loc ? loc : "(null)");
//	loc = ft_setlocale(FT_LC_ALL, "en_US.UTF-8");
//	printf("After setting to 'en_US.UTF-8': %s\n", loc ? loc : "(null)");
//	loc = ft_setlocale(FT_LC_ALL, "");
//	printf("After setting to environment: %s\n", loc ? loc : "(null)");
//	loc = ft_setlocale(FT_LC_NUMERIC, "fr_FR.UTF-8");
//	printf("FT_LC_NUMERIC set to: %s\n", loc ? loc : "(null)");
//	loc = ft_setlocale(FT_LC_NUMERIC, NULL);
//	printf("Current FT_LC_NUMERIC: %s\n", loc ? loc : "(null)");
//	loc = ft_setlocale(999, "C");
//	printf("Invalid category result: %s\n", loc ? loc : "(null)");
//	ft_locale_cleanup();
//	return (0);
//}