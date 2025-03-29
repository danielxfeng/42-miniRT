#include "mini_rt.h"
#include "vars.h"

bool	sp_hit2(t_obj *sphere, t_ray *ray, t_interval *interval,
		t_hit_record *rec)
{
	t_sp_hit_vars	vars;

	if (sphere->material.type_material == LIGHT && ((!LIGHT_VISIBLE
				&& ray->type == CAM_RAY) || ray->type == REFECTION_RAY))
		return (false);
	vars.oc = vec3_sub_vecs(sphere->point, ray->orig);
	vars.a = vec3_length_squared(ray->direc);
	vars.c = vec3_length_squared(vars.oc) - sphere->radius * sphere->radius;
	vars.h = vec3_dot(ray->direc, vars.oc);
	vars.discriminant = vars.h * vars.h - vars.a * vars.c;
	if (vars.discriminant < 0)
		return (false);
	vars.sqrtd = sqrtf(vars.discriminant);
	vars.root = (vars.h - vars.sqrtd) / vars.a;
	if (!interval_surrounds(interval, vars.root))
	{
		vars.root = (vars.h + vars.sqrtd) / vars.a;
		if (!interval_surrounds(interval, vars.root))
			return (false);
	}
	rec->t = vars.root;
	rec->p = ray_at(ray, vars.root);
	vars.outward_normal = vec3_div_vec(vec3_sub_vecs(rec->p, sphere->point),
			sphere->radius);
	rec->material = &sphere->material;
	set_face_normal(*ray, vars.outward_normal, rec);
	rec->u = (atan2f(-vars.outward_normal.z, vars.outward_normal.x) + PI) / (2 * PI);
	rec->v = acosf(-vars.outward_normal.y) / PI;
	return (true);
}

// @details returns if the given sphere is hit by the given ray in given interval.
// and fill the given rec.
//
// The math is:
//
// |P - C|² = R²
//
// For a given ray:
// X(t) = ray.origin + t * ray.direction
// Substituting X into the equation and simplifying gives a quadratic:
// a·t² + b·t + c = 0
//
// The process:
// 1 Compute a, b, c.
// 2. Solve the quadratic equation for t
// 3. For each valid t in the interval:
//    - Compute the hit point and test whether it's valid.
// 4. If hit is valid:
//    - Compute the surface normal at the point
//    - Compute UV coordinates for texturing
bool	sp_hit(t_obj *sphere, t_ray *ray, t_interval *interval,
	t_hit_record *rec)
{
	t_sp_hit_vars	var;

	if (sphere->material.type_material == LIGHT && ((!LIGHT_VISIBLE
				&& ray->type == CAM_RAY) || ray->type == REFECTION_RAY))
		return (false);
	var.oc = vec3_sub_vecs(ray->orig, sphere->point);
	var.a = vec3_length_squared(ray->direc);
	var.b = 2.0f * vec3_dot(ray->direc, var.oc);
	var.c = vec3_length_squared(var.oc) - sphere->radius * sphere->radius;
	if (!quadratic(var.a, var.b, var.c, var.t))
		return false;
	if (interval_surrounds(interval, var.t[0]))
		rec->t = var.t[0];
	else if (interval_surrounds(interval, var.t[1]))
		rec->t = var.t[1];
	else
		return false;
	rec->p = ray_at(ray, rec->t);
	var.outward_normal = vec3_div_vec(vec3_sub_vecs(rec->p, sphere->point), sphere->radius);
	rec->material = &sphere->material;
	set_face_normal(*ray, var.outward_normal, rec);
	rec->u = (atan2f(-var.outward_normal.z, var.outward_normal.x) + PI) / (2.0f * PI);
	rec->v = acosf(-var.outward_normal.y) / PI;
	return true;
}
