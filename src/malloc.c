/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 17:54:51 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/01 12:44:43 by tvallee          ###   ########.fr       */
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

	ft_putstr("malloc: ");
	type = allocs_get_type(block_size(size));
	block = block_fit(block_size(size), type);
	if (block == NULL)
	{
		ft_putendl(" create zone");
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
	ft_puthex((size_t)(block + 1));
	ft_putendl(" OK !");
	ft_putchar(10);
	return (block + 1);
}
