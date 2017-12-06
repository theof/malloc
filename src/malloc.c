/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 17:54:51 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/06 23:54:32 by tvallee          ###   ########.fr       */
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
	static size_t	count = 0;

	size = block_size(size);
	type = allocs_get_type_block(size);
	if (type == 0)
	{
		ft_putstr("malloc(");
		ft_putnbr(size);
		ft_putstr("): ");
		ft_putnbr(TINY_ZONE_SIZE);
		ft_putendl(" tiny zone");
		count += size;
		ft_putnbr(count);
		ft_putendl(" count");
	}
	block = block_fit(size, type);
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
			block = block_create(available, size, type);
		}
	}
	ft_puthex((size_t)(block + 1));
	ft_putendl(" OK !");
	ft_putchar(10);
	return (block + 1);
}
