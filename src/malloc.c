/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 17:54:51 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/28 23:28:32 by tvallee          ###   ########.fr       */
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

	ft_putchar(10);
	ft_putendl("malloc: ");
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
			ft_putstr("new zone: ");
			ft_puthex((size_t)zone);
			ft_putstr(" size: ");
			ft_putnbr(zone_size);
			ft_putstr(" type: ");
			ft_putnbr(type);
			ft_putendl("");
			available = block_init_zone(zone, zone_size, type);
			block = block_create(available, block_size(size), type);
		}
	}
	else
	{
		ft_putstr("new block: ");
		ft_puthex((size_t)block);
		ft_putstr(" size: ");
		ft_putnbr(block->size);
		ft_putstr(" type: ");
		ft_putnbr(type);
		ft_putchar(10);
	}
	return (block + 1);
}
