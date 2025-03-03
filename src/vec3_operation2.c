/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operation2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:40:49 by mquero            #+#    #+#             */
/*   Updated: 2025/03/01 20:51:57 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void    get_point_from_ray(t_vec3 *point ,t_ray ray, float t)
{
    point->x = ray.orig.x + (t * ray.direc.x);
    point->y = ray.orig.y + (t * ray.direc.y);
    point->z = ray.orig.z + (t * ray.direc.z);
}

t_vec3    multiply_colors(t_vec3 startcolor , t_vec3 endcolor)
{
    t_vec3 new_color;

    new_color.x = startcolor.x  * endcolor.x;
    if (new_color.x > 1)
        new_color.x = 1;
    new_color.y = startcolor.y * endcolor.y;
    if (new_color.y > 1)
        new_color.y = 1;
    new_color.z = startcolor.z * endcolor.z;
    if (new_color.z > 1)
        new_color.z = 1;
    return (new_color);
}

void    multiply_vec_by_num(t_vec3 *vec3, float mult)
{
    vec3->x = (float)vec3->x * mult;
    vec3->y = (float)vec3->y * mult;
    vec3->z = (float)vec3->z * mult;
}
