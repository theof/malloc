/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 08:14:23 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/24 08:31:06 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_allocs	g_allocs[3] = {
	[E_ALLOC_TINY] = {
		.zones = NULL,
		.free_blocks = NULL,
	},
	[E_ALLOC_SMALL] = {
		.zones = NULL,
		.free_blocks = NULL,
	},
	[E_ALLOC_LARGE] = {
		.zones = NULL,
		.free_blocks = NULL,
	},
};

int	allocs_get_type(size_t request_size)
{
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

	return ();
}
