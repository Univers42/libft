/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catch_event.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 09:34:39 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 13:34:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "input_handler.h"


// Static helpers for each event type
static int is_zoom(int keycode)      { return what_category(keycode, EVENT_ZOOM); }
static int is_win(int keycode)       { return what_category(keycode, EVENT_WINDOW); }
static int is_mouse(int keycode)     { return what_category(keycode, EVENT_MOUSE); }
static int is_exit(int keycode)      { return what_category(keycode, EVENT_EXIT); }
static int is_interface(int keycode) { return what_category(keycode, EVENT_ALL); }

// Return int (not function pointer) for event checks
int is_e_zoom(int keycode)      { return is_zoom(keycode); }
int is_e_win(int keycode)       { return is_win(keycode); }
int is_e_mouse(int keycode)     { return is_mouse(keycode); }
int is_e_exit(int keycode)      { return is_exit(keycode); }
int is_e_interface(int keycode) { return is_interface(keycode); }
