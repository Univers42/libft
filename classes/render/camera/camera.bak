/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:03:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/28 11:03:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	set_control_palette(mlx_key_data_t keydata, t_fdf *fdf)
{
	if (keydata.key == MLX_KEY_0)
		fdf->control.palette = 0;
	if (keydata.key == MLX_KEY_1)
		fdf->control.palette = 1;
	if (keydata.key == MLX_KEY_2)
		fdf->control.palette = 2;
	if (keydata.key == MLX_KEY_3)
		fdf->control.palette = 3;
	if (keydata.key == MLX_KEY_4)
		fdf->control.palette = 4;
	if (keydata.key == MLX_KEY_5)
		fdf->control.palette = 5;
	modify_mesh(fdf);
}
/*
 * Modifyies mesh if exact key is pressed
*/
void	set_control_movement(mlx_key_data_t keydata, t_fdf *fdf)
{
	if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_W)
	{
		fdf->control.vert -= 5;
		modify_mesh(fdf);
	}
	if (keydata.key == MLX_KEY_DOWN || keydata.key == MLX_KEY_S)
	{
		fdf->control.vert += 5;
		modify_mesh(fdf);
	}
	if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_A)
	{
		fdf->control.horiz -= 5;
		modify_mesh(fdf);
	}
	if (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_D)
	{
		fdf->control.horiz += 5;
		modify_mesh(fdf);
	}
}

void	set_control_view(mlx_key_data_t keydata, t_fdf *fdf)
{
	if (keydata.key == MLX_KEY_T)
		fdf->control.perspective = TOP_VIEW;
	if (keydata.key == MLX_KEY_I)
		fdf->control.perspective = ISOMETRIC;
	modify_mesh(fdf);
}

void	set_control_zoom(double x_delta, double y_delta, void *fdf_void)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)fdf_void;
	if (y_delta > 0)
	{
		if (fdf->control.zoom <= 150)
			fdf->control.zoom += 1;
	}
	else if (y_delta < 0)
	{
		if (fdf->control.zoom >= 0)
			fdf->control.zoom -= 1;
	}
	if (x_delta < 0)
	{
		if (fdf->control.zoom <= 150)
			fdf->control.zoom += 1;
	}
	else if (x_delta > 0)
	{
		if (fdf->control.zoom >= 0)
			fdf->control.zoom -= 1;
	}
	modify_mesh(fdf);
}

/*
 * Modifyies mesh if exact key is pressed and value is in range
*/
void	set_control_point_resolution(mlx_key_data_t keydata, t_fdf *fdf)
{
	if (keydata.key == MLX_KEY_V)
	{
		if (fdf->control.points_resolution >= 1)
		{
			fdf->control.points_resolution -= 1;
			modify_mesh(fdf);
		}
	}
	if (keydata.key == MLX_KEY_B)
	{
		if (fdf->control.points_resolution <= 100)
		{
			fdf->control.points_resolution += 1;
			modify_mesh(fdf);
		}
	}
}

void	move_img(mlx_key_data_t keydata, void *param)
{
	t_fdf	*fdf;
	keys_t	key;

	fdf = param;
	key = keydata.key;
	if (key == MLX_KEY_A && keydata.action == MLX_RELEASE
		&& keydata.modifier == MLX_CONTROL)
		puts("Gotta grab it all!");
	if (key == MLX_KEY_ESCAPE)
		mlx_close_window(fdf->mlx);
	if ((key >= 262 && key <= 265))
		set_control_movement(keydata, fdf);
	else if (key == MLX_KEY_T || key == MLX_KEY_I)
		set_control_view(keydata, fdf);
	else if (key == MLX_KEY_N || key == MLX_KEY_M || key == 267
		|| key == 334 || key == 266)
		set_control_height(keydata, fdf);
	else if (key == MLX_KEY_Z || key == MLX_KEY_X)
		set_control_rotation(keydata, fdf);
	else if (key >= MLX_KEY_0 && key <= MLX_KEY_5)
		set_control_palette(keydata, fdf);
	else if (key >= MLX_KEY_B && key <= MLX_KEY_V)
		set_control_point_resolution(keydata, fdf);
}


void	set_control_rotation(mlx_key_data_t keydata, t_fdf *fdf)
{
	if (keydata.key == MLX_KEY_Z)
		fdf->control.rot_angle += 0.04;
	if (keydata.key == MLX_KEY_X)
		fdf->control.rot_angle -= 0.04;
	modify_mesh(fdf);
}

void	set_control_height(mlx_key_data_t keydata, t_fdf *fdf)
{
	if (keydata.key == MLX_KEY_KP_SUBTRACT || keydata.key == MLX_KEY_MINUS
		|| keydata.key == MLX_KEY_N || keydata.key == MLX_KEY_PAGE_DOWN)
	{
		fdf->control.height -= 1;
		modify_mesh(fdf);
	}
	if (keydata.key == MLX_KEY_KP_ADD || keydata.key == MLX_KEY_PAGE_UP
		|| keydata.key == MLX_KEY_M)
	{
		fdf->control.height += 1;
		modify_mesh(fdf);
	}
}