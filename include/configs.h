/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 22:13:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 18:30:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGS_H
# define CONFIGS_H

/**
 * enable a builting variable names LINENO (short for line number)
 * lineno is the integer that a parser or reader increments to track
 * the current input/source line
 * EXAMPLE:
 * linenove is the fixed buffer that hold the variable text in the same
 * name=value format used for all variables initially "LINENO="
 */
# define WITH_LINENO
#endif
