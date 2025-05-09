/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 09:40:25 by mquero            #+#    #+#             */
/*   Updated: 2025/04/09 09:40:27 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "mini_rt.h"
#include <sys/time.h>

void			camera_init(t_cam *c);
t_ray			camera_get_ray(t_cam *c, int i, int j);
t_color			camera_ray_color(t_info *info, t_ray ray, t_obj **world,
					int depth);

unsigned int	get_color(t_vec3 vec)
{
	return (((uint8_t)(vec.x * 255) << 24) | ((uint8_t)(vec.y
				* 255) << 16) | ((uint8_t)(vec.z * 255) << 8) | 255);
}
// @details
// Signals the worker threads to start rendering a new frame.
// Sets `abort_signal` to 1, which tells threads that rendering should proceed.
// Increments `work_available` to notify all threads that a new frame is ready.
// Threads are waiting for `work_available` to change before proceeding.

void	camera_render(t_info *info)
{
	atomic_store(&info->pool.abort_signal, 1);
	atomic_fetch_add(&info->pool.work_available, 1);
}

void	camera_start(t_info *info)
{
	info->c.image_height = DEFAULT_HEIGHT;
	info->c.image_width = DEFAULT_WIDTH;
	info->mlx = mlx_init(DEFAULT_WIDTH, DEFAULT_HEIGHT, "KD MiniRT", true);
	info->img = mlx_new_image(info->mlx, info->c.image_width,
			info->c.image_height);
	if (!info->img || mlx_image_to_window(info->mlx, info->img, 0, 0) < 0)
		free_all(info);
	init_thread_pool(info);
	camera_init(&info->c);
	camera_render(info);
}

void	camera_resize_screen(t_info *info, int image_width, int image_height)
{
	wait_for_threads(info);
	info->c.image_height = image_height / THREADS_AMOUNT * THREADS_AMOUNT;
	info->c.image_width = image_width / THREADS_AMOUNT * THREADS_AMOUNT;
	if (!mlx_resize_image(info->img, info->c.image_width, info->c.image_height))
		free_all(info);
	camera_init(&info->c);
	camera_render(info);
}
