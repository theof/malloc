/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/22 22:07:52 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/23 03:17:17 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_zone	g_zones[3] = {
	[E_ZONE_TINY] = {
		.next = &(g_zones[E_ZONE_TINY]),
		.prev = &(g_zones[E_ZONE_TINY]),
		.block = {
			.size = MAGIC_STATIC_ZONE
		}
	},
	[E_ZONE_SMALL] = {
		.next = &(g_zones[E_ZONE_SMALL]),
		.prev = &(g_zones[E_ZONE_SMALL]),
		.block = {
			.size = MAGIC_STATIC_ZONE
		}
	},
	[E_ZONE_LARGE] = {
		.next = &(g_zones[E_ZONE_LARGE]),
		.prev = &(g_zones[E_ZONE_LARGE]),
		.block = {
			.size = MAGIC_STATIC_ZONE
		}
	},
};

size_t	align_size(size_t size)
{
	size_t	page_size;
	size_t	truncated;

	page_size = getpagesize();
	truncated = size & (page_size - 1);
	if (truncated < size)
		return (truncated + page_size);
	else
		return (truncated);
}

void	zone_map(t_zone **dst, size_t size)
{
	t_zone	*new;
	t_zone	*link;
	size_t	zone_size;

	if (size > SMALL_SIZE)
	{
		link = &(g_zones[E_ZONE_LARGE]);
		zone_size = get_large_zone_size(size + sizeof(t_zone));
	}
	else if (size > TINY_SIZE)
	{
		link = &(g_zones[E_ZONE_SMALL]);
		zone_size = SMALL_ZONE_SIZE;
	}
	else
	{
		link = &(g_zones[E_ZONE_TINY]);
		zone_size = TINY_ZONE_SIZE;
	}
	new = mmap(0, zone_size, PROT_READ_WRITE, MAP_ANON_PRIVATE, -1, 0);
	new->prev = link->prev;
	new->next = link;
	link->prev->next = new;
	link->prev = new;
	return (new);
}
