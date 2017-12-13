/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 17:54:51 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/13 17:57:17 by tvallee          ###   ########.fr       */
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

	size = block_size(size);
	type = allocs_get_type_block(size);
	ft_putstr("malloc(");
	ft_putnbr(size);
	ft_putendl("):");
	block = block_fit(size, type);
	if (block == NULL)
	{
		ft_putstr("\tnew zone: ");
		zone_size = zone_map(&zone, size, type);
		if (zone == NULL)
		{
			ft_putendl("got NULL in mmap !!");
			return (NULL);
		}
		else
		{
			ft_puthex((size_t)zone);
			ft_putchar(10);
			available = block_init_zone(zone, zone_size, type);
			block = block_create(available, size, type);
		}
	}
	ft_puthex((size_t)block);
	ft_putstr(" => ");
	if (get_next_block(block))
	{
		ft_puthex((size_t)get_next_block(block));
	}
	ft_putendl(" OK !\n");
	return (block + 1);
}
