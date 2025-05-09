/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mquero <mquero@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 09:47:02 by mquero            #+#    #+#             */
/*   Updated: 2025/04/09 09:47:03 by mquero           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

// @brief to set the uv for plane.
static void	set_uv(t_obj *plane, t_hit_record *rec)
{
	t_vec3	offset;
	t_vec3	u_axis;
	t_vec3	v_axis;

	offset = vec3_sub_vecs(rec->p, plane->point);
	if (fabsf(plane->normal.y) < 0.999f)
		u_axis = vec3_unit(vec3_cross(vec3_new(0, 1, 0), plane->normal));
	else
		u_axis = vec3_unit(vec3_cross(vec3_new(1, 0, 0), plane->normal));
	v_axis = vec3_unit(vec3_cross(plane->normal, u_axis));
	rec->u = fmodf(vec3_dot(offset, u_axis) * CHECK_SCALE, 1.0f);
	rec->v = fmodf(vec3_dot(offset, v_axis) * CHECK_SCALE, 1.0f);
	if (rec->u < 0)
		rec->u += 1.0f;
	if (rec->v < 0)
		rec->v += 1.0f;
	rec->uv_chess_board = true;
}

bool	pl_hit(t_obj *plane, t_ray *ray, t_interval *interval,
		t_hit_record *rec)
{
	t_vec3	plane_ray_vec;
	float	dot_plane;
	float	result;

	dot_plane = vec3_dot(ray->direc, plane->normal);
	if (dot_plane == 0)
		return (false);
	plane_ray_vec = plane->point;
	plane_ray_vec = vec3_sub_vecs(plane_ray_vec, ray->orig);
	result = vec3_dot(plane_ray_vec, plane->normal);
	result = result / dot_plane;
	if (!interval_surrounds(interval, result))
		return (false);
	rec->t = result;
	rec->p = ray_at(ray, result);
	rec->normal = plane->normal;
	if (vec3_dot(rec->normal, ray->direc) > 0)
		rec->normal = vec3_mul_vec(rec->normal, -1);
	set_face_normal(*ray, rec->normal, rec);
	rec->material = &plane->material;
	set_uv(plane, rec);
	return (true);
}
