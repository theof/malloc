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

void	*malloc(size_t size)
{
	t_zone			*zone;
	t_block			*block;
	t_block_free	*available;
	unsigned		type;
	size_t			zone_size;

	ft_putendl("malloc: ");
	type = allocs_get_type(block_size(size));
	block = block_fit(block_size(size), type);
	if (block == NULL)
	{
		zone_size = zone_map(&zone, size, type);
		if (zone == NULL)
			return (NULL);
		else
		{
			available = block_init_zone(zone, zone_size, type);
			block = block_create(available, block_size(size), type);
		}
	}
	ft_puthex((size_t)(block+1));
	ft_putstr(" : ");
	ft_putnbr(size);
	ft_putchar(10);
	ft_putendl("finished");
	ft_puthex((size_t)(block + 1));
	ft_putendl(" block + 1");
	return (block + 1);
}
