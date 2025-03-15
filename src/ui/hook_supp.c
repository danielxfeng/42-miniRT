#include "mini_rt.h"

void	print_position(t_info *info)
{
	int	x;
	int	y;

	mlx_get_mouse_pos(info->mlx, &x, &y);
	printf("Mouse: x %d, y: %d\n", x, y);
}

void	handle_transition_event(t_info *info, keys_t key)
{
	t_vec3 step;

	if (key == MLX_KEY_LEFT)
		step = vec3_mul_vec(info->camera.u, -MOVE_STEP);
	else if (key == MLX_KEY_RIGHT)
		step = vec3_mul_vec(info->camera.u, MOVE_STEP);
	else if (key == MLX_KEY_UP)
		step = vec3_mul_vec(info->camera.w, -MOVE_STEP);
	else if (key == MLX_KEY_DOWN)
		step = vec3_mul_vec(info->camera.w, MOVE_STEP);
	info->camera.point = vec3_add_vecs(info->camera.point, step);
	camera_render(info);
}
