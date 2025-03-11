
#include "MLX42/MLX42.h"
#include "mini_rt.h"

void	print_vec3(t_vec3 vec3)
{
	printf("x : %f\n", vec3.x);
	printf("y : %f\n", vec3.y);
	printf("z : %f\n", vec3.z);
}



void	handle_win_close_event(void *param)
{
	t_info	*info;

	info = (t_info *)param;
	free_arena_exit(info);
}

int	main(int argc, char **argv)
{
	t_info	info;
	int		i;

	i = 0;
	if (argc != 2 || check_if_rt(argv[1]) == 0)
		throw_error(0);
	ft_bzero(&info, sizeof(t_info));
	info.arena = arena_init(10240);
	parse(argv[1], &info);
	//info.img = mlx_new_image(info.mlx, WIDTH, HEIGHT);
	 //if (!info.img || (mlx_image_to_window(info.mlx, info.img, 0, 0) < 0))
	// return (0);
	// print_vec3(info.obj[8].point);

	// print_vec3(info.c.orient);
	printf("NUMBER OF OBJECTS %u\n", info.obj_count);
	print_vec3(info.obj[0].rgb);
	print_vec3(info.obj[0].point);
	print_vec3(info.obj[0].normal);
	printf("%f\n", info.obj[0].height);
	printf("%f\n", info.obj[0].radius);
	//print_vec3(info.l[0].rgb);
	//throw_rays(&info);
	camera_start(&info,info.c.point ,info.c.fov, info.c.orient); // call this function to start.
	//ft_memset(info.img->pixels, 255, info.img->width * info.img->height * sizeof(int32_t));
	//mlx_image_to_window(info.mlx, info.img, 0 , 0);
	mlx_close_hook(info.mlx, handle_win_close_event, &info);
	mlx_loop(info.mlx);
	// free_all(&info);
	free_arena_exit(&info);
	return (0);
}
