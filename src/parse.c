/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:40:27 by mquero            #+#    #+#             */
/*   Updated: 2025/03/03 13:57:10 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static  void assign_camera_info(t_info *info, char **split)
{
    char  **vec;

    info->c.fov = strtof(split[3], NULL);
    vec = ft_split(split[1], ',');
    if (!vec)
        free_arena_exit(info);
    new_vec3(&(info->c).point, vec, false);
    vec = ft_split(split[2], ',');
    if (!vec)
        free_arena_exit(info);
    new_vec3(&(info->c).orient, vec, false);
}

static  void assign_ambient_info(t_info *info, char **split)
{
    char  **rgb;

    info->a.ratio = strtof(split[1], NULL);
    rgb = ft_split(split[2], ',');
    if (!rgb)
        free_arena_exit(info);
    new_vec3(&(info->a).rgb, rgb, true);
}

static  void assign_light_info(t_info *info, char **split)
{
    char  **vec;

    info->l.br_ratio = strtof(split[2], NULL);
    vec = ft_split(split[3], ',');
    if (!vec)
        free_arena_exit(info);
    new_vec3(&(info->l).rgb, vec, true);
    vec = ft_split(split[1], ',');
    if (!vec)
        free_arena_exit(info);
    new_vec3(&(info->l).point, vec, false);
}

static  void assign_all(t_info *info, char **split)
{
    if (ft_strncmp(split[0], "\n", ft_strlen(split[0])) == 0)
        return ;
    else if (ft_strncmp(split[0], "A", ft_strlen(split[0])) == 0)
        assign_ambient_info(info, split);
    else if (ft_strncmp(split[0], "C", ft_strlen(split[0])) == 0)
        assign_camera_info(info, split);
    else if (ft_strncmp(split[0], "L", ft_strlen(split[0])) == 0)
        assign_light_info(info, split);
    else if (ft_strncmp(split[0], "pl", ft_strlen(split[0])) == 0)
        create_object_info(info, split);
    else if (ft_strncmp(split[0], "sp", ft_strlen(split[0])) == 0)
        create_object_info(info, split);
    else if (ft_strncmp(split[0], "cy", ft_strlen(split[0])) == 0)
        create_object_info(info, split);
    else
    {
        arena_free(info->arena);
        freesplit(split);
        throw_error(2);
    }
}

void    parse(char *file, t_info *info)
{
    int fd;
    char *input;
    char **split;

    info->obj = arena_alloc(info->arena, sizeof(t_obj) * 100);
    fd = open(file, O_RDONLY);
    if (fd == -1)
        throw_error(1);
    input = get_next_line(fd);
    while (input != NULL)
    {
        split = ft_split(input, ' ');
        free(input);
        if (!split)
            free_arena_exit(info);
        assign_all(info, split);
        freesplit(split);
        input = get_next_line(fd);
    }
    //info->pl = ft_memmove(info->sp + info->sp_count, info->pl, sizeof(t_pl) * info->pl_count);
    //info->cy = ft_memmove(info->pl + info->pl_count, info->cy, sizeof(t_cy) * info->cy_count);
}
