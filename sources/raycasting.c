#include "wolf3d.h"

t_ray	*init_ray(char* type)
{
	t_ray *ray;

	ray = (t_ray *)malloc(sizeof(t_ray) * 10);
	ray->dist = 9999;
	ray->offset = 0;
	ray->texture = NULL;
	ray->type = type;
	ray->count = 0;
	return (ray);
}

int		is_wall(t_map *map, t_fpoint *pos, t_ray *ray, double angle, t_player *player, t_fpoint *step)
{
	int		y;
	int		x;
	t_node	node;
	int		*index;
	t_ray   *current_ray;

	if (pos->x < 0 || pos->y < 0)
		return (1);
	if (pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	y = pos->y / TEXTURE_SIZE;
	x = pos->x / TEXTURE_SIZE;
	node = map->nodes[y][x];
	index = &ray[0].count;
	current_ray = &ray[*index];
	if (node.type == 2)
	{
		current_ray->texture = node.texture[0];
		current_ray->offset = (ft_strcmp(ray->type, "H") == 0) ?
				(int)pos->x % TEXTURE_SIZE:
				(int)pos->y % TEXTURE_SIZE;
/*		current_ray->dist = (ft_strcmp(ray->type, "H") == 0) ?
				fabs(((pos->y + step->y * 0.5) - player->y) / sin(angle)):
				fabs(((pos->x + step->x * 0.5) - player->x) / cos(angle));*/
		current_ray->dist = (ft_strcmp(ray->type, "H") == 0) ?
							fabs(((pos->y + step->y * 0.5) - player->y) / player->sin_table[(int)angle]):
							fabs(((pos->x + step->x * 0.5) - player->x) / player->cos_table[(int)angle]);
		ray[0].count++;
	}
	else if (node.type == 3)
	{
		current_ray->offset = (ft_strcmp(ray->type, "H") == 0) ?
				(int)(pos->x + step->x * 0.5) % TEXTURE_SIZE:
				(int)(pos->y + step->y * 0.5) % TEXTURE_SIZE;
		if (current_ray->offset >= player->door_frame)
		{
			current_ray->offset -= player->door_frame;
			current_ray->texture = node.texture[0];
/*			current_ray->dist = (ft_strcmp(ray->type, "H") == 0) ?
					fabs(((pos->y + step->y * 0.5) - player->y) / sin(angle)):
					fabs(((pos->x + step->x * 0.5) - player->x) / cos(angle));*/
			current_ray->dist = (ft_strcmp(ray->type, "H") == 0) ?
								fabs(((pos->y + step->y * 0.5) - player->y) / player->sin_table[(int)angle]):
								fabs(((pos->x + step->x * 0.5) - player->x) / player->cos_table[(int)angle]);
			ray[0].count++;
			return (1);
		}
	}
	else if (node.type == 1)
	{
		current_ray->texture = (ft_strcmp(ray->type, "H") == 0) ?
				node.texture[(sin(angle) > 0 ? 2 : 1)]:
				node.texture[(cos(angle) < 0 ? 0 : 3)];
/*		current_ray->texture = (ft_strcmp(ray->type, "H") == 0) ?
							   node.texture[player->sin_table[(int)angle] > 0 ? 2 : 1]:
							   node.texture[(player->cos_table[(int)angle] < 0 ? 0 : 3)];*/
		current_ray->dist = (ft_strcmp(ray->type, "H") == 0) ?
				fabs((player->y - pos->y) / sin(angle)):
				fabs((player->x - pos->x) / cos(angle));
		current_ray->offset = (ft_strcmp(ray->type, "H") == 0) ?
				(int)pos->x % TEXTURE_SIZE :
				(int)pos->y % TEXTURE_SIZE;
		ray[0].count++;
	}
	return (0);
}

t_ray	*cast_ray_horz(t_map *map, t_player *player, double angle)
{
	t_fpoint	end;
	t_fpoint	step;
	t_ray		*ray;
	double		old_angle;

	ray = init_ray("H");
	old_angle = angle;
	angle = angle * M_PI_180;
	if (sin(angle) == 0)
		return (ray);
	end.y = (sin(angle) < 0) ?
			player->y / 64 * 64 - 0.001:
			player->y / 64 * 64 + 64.001;
	step.y = (sin(angle) < 0) ? -64 : 64;
	step.x = -step.y / tan(-angle);
	end.x = player->x + (end.y - player->y) / tan(angle);
	while (!is_wall(map, &end, ray, angle, player, &step))
	{
		end.x += step.x;
		end.y += step.y;
	}
	return (ray);
}

t_ray	*cast_ray_vert(t_map *map, t_player *player, double angle)
{
	t_fpoint	end;
	t_fpoint	step;
	t_ray		*ray;

	ray = init_ray("V");
	angle = angle * M_PI_180;
	if (cos(angle) == 0)
		return (ray);
	end.x = (cos(angle) > 0) ?
			player->x / 64 * 64 + 64.001:
			player->x / 64 * 64 - 0.001;
	step.x = (cos(angle) > 0) ? 64 : -64;
	step.y = -step.x * tan(-angle);
	end.y = player->y + (end.x - player->x) * tan(angle);
	while (!is_wall(map, &end, ray, angle, player, &step))
	{
		end.x += step.x;
		end.y += step.y;
	}
	return (ray);
}

t_ray	*get_ray(t_map *map, t_player *player, double angle)
{
	t_ray	*ray_vert;
	t_ray	*ray_horz;

	ray_horz = cast_ray_horz(map, player, angle);
	ray_vert = cast_ray_vert(map, player, angle);
	if (ray_horz->dist <= ray_vert->dist)
	{
		//ray_horz->dist *= cos(fabs(angle - player->direction) * M_PI_180);
		ray_horz->dist *= player->cos_table180[(int)(fabs(angle - player->direction))];
		free(ray_vert);
		return (ray_horz);
	}
	else
	{
		//ray_vert->dist *= cos(fabs(angle - player->direction) * M_PI_180);
		ray_vert->dist *= player->cos_table180[(int)(fabs(angle - player->direction))];
		free(ray_horz);
		return (ray_vert);
	}
}

void	cast_single_ray(t_app *app, int x, float angle)
{
	int		slice_height;
	t_ray	*rays;
	int 	i;

	rays = get_ray(app->map, app->player, angle);
	i = rays->count;
	while (i > 0)
	{
		slice_height = (int)(64 / rays[i - 1].dist * app->sdl->dist_to_pp);
		draw_column(app->sdl, &rays[i - 1], x, slice_height, i - 1);
		i--;
	}
	free(rays);
}

//void	draw_objects(t_app *app)
//{
//	int i;
//	float dist;
//	int slice_height;
//	t_ray ray;
//
//	i = 0;
//	while (i < app->map->obj_count)
//	{
//		dist = sqrt(ft_powd(app->player->x - app->map->objects[i].x, 2) + ft_powd(app->player->y - app->map->objects[i].y, 2));
//		slice_height = (int)(64 / dist * app->sdl->dist_to_pp);
//		ray.texture = app->map->objects[i].texture;
//		ray.dist = dist;
//		ray.offset = 0;
//		while (ray.offset < app->sdl->width)
//		{
//			draw_obj_column(app->sdl, &ray, app->map->objects[i].x + ray.offset, slice_height);
//			ray.offset++;
//		}
//		i++;
//	}
//}

void	create_field_of_view(t_app *app)
{
	float	angle;
	float	next_angle;
	int		x;
	double	fov;

	fov = 60;
	angle = app->player->direction - (fov / 2.0);
	next_angle = fov / app->sdl->width;
	x = 0;
	while (x < app->sdl->width)
	{
		cast_single_ray(app, x, angle);
		angle = angle + next_angle;
		x++;
	}
	//draw_objects(app);
}
