/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 17:54:51 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/27 23:07:13 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "malloc.h"
#include "libft.h"

t_block	*block_fit(size_t size, unsigned type)
{
	/*
	t_zone	*zone;
	t_block	*block;

	zone = g_allocs[type].zones;
	while (zone != NULL)
	{
		zone = zone->next;
	}*/
	(void)size;
	(void)type;
	return (NULL);
}

void	*malloc(size_t size)
{
	t_zone		*zone;
	t_block		*block;
	unsigned	type;
	size_t		zone_size;

	type = allocs_get_type(size);
	block = block_fit(block_size(size), type);
	if (block == NULL)
	{
		zone_size = zone_map(&zone, size, type);
		if (zone == NULL)
			return (NULL);
		else
			block = block_init_zone(zone, zone_size, block_size(size));
	}
	return (block + 1);
}
