/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:01:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 00:44:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HANDLER_H
# define INPUT_HANDLER_H

# include "camera.h"

typedef struct s_input_handler
{
	void	(*key_press)(struct s_input_handler *self, int keycode);
	void	(*key_release)(struct s_input_handler *self, int keycode);
	void	(*mouse_move)(struct s_input_handler *self, int x, int y);
	void	(*mouse_click)(struct s_input_handler *self, int button, int x, int y);
	t_camera *camera;
}	t_input_handler;

t_input_handler	*InputHandler_new(t_camera *camera);
void	input_handler_register(t_window *window, t_input_handler *handler);
void	input_handler_unregister(t_window *window, t_input_handler *handler);
t_camera *input_handler_get_camera(t_input_handler *handler);

#endif