/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 17:54:51 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 15:15:34 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "malloc.h"
#include "libft.h"

void	*malloc(size_t size)
{
	t_zone			*zone;
	t_block			*block;
	t_block_free	*available;
	unsigned		type;
	size_t			zone_size;

	type = allocs_get_type(block_size(size));
	block = block_fit(block_size(size), type);
	if (block == NULL)
	{
		zone_size = zone_map(&zone, size, type);
		if (zone == NULL)
		{
			return (NULL);
		}
		else
		{
			available = block_init_zone(zone, zone_size, type);
			block = block_create(available, block_size(size), type);
		}
	}
	return (block + 1);
}
