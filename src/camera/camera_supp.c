/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_supp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xifeng <xifeng@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 09:46:15 by mquero            #+#    #+#             */
/*   Updated: 2025/04/09 14:45:25 by xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "vars.h"

t_color	get_phong_color(t_info *info, t_get_light_vars *var);
t_color	get_phong_ambient(t_info *info);
t_color	camera_ray_color(t_info *info, t_ray ray, t_obj **world, int depth);
void	init_shadow_ray(t_get_light_vars *var, t_ray *ray, t_hit_record *rec);

// @details apply the math to calculate the properties of the camera.
//
// look_at = point + orint
// vfov = 2 * arctan( tan(hov / 2) / aspect_ratio )
// w = - orint
// u = unit(cross(vup, w));
// v = cross(w, u);
// vup = -(cross(unit(cross((0, 1, 0), w)), w))
//  - When the vup is 0, 1, 0, the vup = v.
//  - So we calculate the vup by assuming the vup = 0, 1, 0,
//  - then the v value will be the vup.
// viewport_height = 2 * tan(vfov / 2) * focal_length
// pixel_delta_u = viewport_u / image_width
// top_left = look_at - viewport_u / 2 - viewport_v / 2
// pixel00_loc = top_left and shift by half pixel.
void	camera_init(t_cam *c)
{
	t_camera_init_vars	var;

	var.look_at = vec3_add_vecs(c->point, c->orient);
	var.aspect_ratio = (float)c->image_width / c->image_height;
	var.vfov = 2.0f * atanf(tanf(degree_to_radius(c->hov) / 2)
			/ var.aspect_ratio);
	c->w = vec3_flip_minus(c->orient);
	var.right = vec3_unit(vec3_cross(vec3_new(0, 1, 0), c->w));
	var.vup = vec3_flip_minus(vec3_cross(var.right, c->w));
	c->u = vec3_unit(vec3_cross(var.vup, c->w));
	var.v = vec3_cross(c->w, c->u);
	var.focal_length = vec3_length(vec3_sub_vecs(c->point, var.look_at));
	var.viewport_height = 2.0 * tanf(var.vfov / 2) * var.focal_length;
	var.viewport_width = var.aspect_ratio * var.viewport_height;
	var.viewport_u = vec3_mul_vec(c->u, var.viewport_width);
	var.viewport_v = vec3_mul_vec(var.v, -var.viewport_height);
	c->pixel_delta_u = vec3_div_vec(var.viewport_u, c->image_width);
	c->pixel_delta_v = vec3_div_vec(var.viewport_v, c->image_height);
	var.top_left = vec3_sub_vecs(var.look_at,
			vec3_add_vecs(vec3_div_vec(var.viewport_u, 2),
				vec3_div_vec(var.viewport_v, 2)));
	c->pixel00_loc = vec3_add_vecs(var.top_left,
			vec3_mul_vec(vec3_add_vecs(c->pixel_delta_u, c->pixel_delta_v),
				0.5));
}

// @details to init a camera ray.
//
// pixel_sample: since a pixel represents an area, not a single point,
//   we start from pixel00_loc, center of the top-left pixel in the image plane.
//   then the shift based on i and j, scaling by the number of world space units
//   per image plane pixel.
t_ray	camera_get_ray(t_cam *c, int i, int j)
{
	t_ray	ray;
	t_vec3	pixel_sample;

	pixel_sample = vec3_add_vecs(vec3_add_vecs(c->pixel00_loc,
				vec3_mul_vec(c->pixel_delta_u, i)),
			vec3_mul_vec(c->pixel_delta_v, j));
	ray.orig = c->point;
	ray.direc = vec3_unit(vec3_sub_vecs(pixel_sample, c->point));
	ray.type = CAM_RAY;
	return (ray);
}

// @details to handle the shadow ray.
//
// Init the light to `ambient light`.
// When an object with diffuse material is hit, then send only shadow rays
// to light.
//  - shadow rays are from the intersection point to each light,
//    so it's an iteration process.
//  - hitting the light means with light, then try to apply `phong model`.
//    https://en.wikipedia.org/wiki/Phong_reflection_model
//    to accumulate the diffuse light and specular light.
t_color	camera_send_shadow_rays(t_info *info, t_ray *ray, t_hit_record *rec)
{
	t_get_light_vars	var;
	t_color				color;
	int					i;
	bool				is_shadow;

	color = get_phong_ambient(info);
	init_shadow_ray(&var, ray, rec);
	i = 0;
	is_shadow = true;
	while (i < info->light_count)
	{
		var.interval.max = INFINITY;
		var.shadow_ray.direc = vec3_unit(vec3_sub_vecs(info->lights[i]->point,
					rec->p));
		if (world_hit(info, &var.shadow_ray, &var.shadow_rec, &var.interval)
			&& var.shadow_rec.material->type_material == LIGHT)
		{
			is_shadow = false;
			color = vec3_add_vecs(color, get_phong_color(info, &var));
		}
		++i;
	}
	color = vec3_mul_colors(rec->material->shading(info,
				rec->material, rec), color);
	return (color);
}

// @details Monte Carlo Path Tracing is applied for calculating the reflection.
// https://en.wikipedia.org/wiki/Path_tracing
//
// there are 3 base cases to stop the recurssive call:
// 1 the scatter returns false (diffuse materail), returns black.
// 2 the materail is LIGHT
// 3 the attenuation is near black.
t_color	camera_send_reflect_rays(t_info *info, t_ray *ray, t_hit_record *rec,
		int depth)
{
	t_ray	scattered;
	t_color	attenuation;

	if (rec->material->scatter(ray, rec, &attenuation, &scattered))
	{
		if (rec->material->type_material == LIGHT
			|| vec3_near_black(attenuation))
			return (attenuation);
		return (vec3_mul_vecs(camera_ray_color(info, scattered, &info->obj,
					depth - 1), attenuation));
	}
	return (vec3_black());
}

// @brief to calculate the color of a ray.
//
// If a ray hits nothing, then it's the sky.
// If a ray hits a diffuse material, apply the phong model.
//   - calculate the color by sending shadow rays.
// If a ray hits a metal or glass, apply the monte carlo path tracing.
//   - calculate the color by sending refection rays.
//
// @param info: the pointer to the state of the program.
// @param ray: the ray to calculate.
// @param world: the objects.
// @param depth: the rest depth of recursive call.
// @return the color.
t_color	camera_ray_color(t_info *info, t_ray ray, t_obj **world, int depth)
{
	t_hit_record	rec;
	t_interval		interval;
	t_color			res;

	if (depth <= 0)
		return (vec3_new(0, 0, 0));
	interval = interval_default();
	if (world_hit(info, &ray, &rec, &interval))
	{
		if (rec.material->type_material == DIFFUSE)
			return (camera_send_shadow_rays(info, &ray, &rec));
		else
			return (camera_send_reflect_rays(info, &ray, &rec, depth));
	}
	return (vec3_sky());
}
