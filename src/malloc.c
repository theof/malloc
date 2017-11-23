/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 17:54:51 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/23 04:21:44 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"
#include <unistd.h>

void	*malloc(size_t size)
{
	t_zone	*zone;
	t_block	*block;

	block = block_fit(size);
	if (block == NULL)
	{
		zone_map(&zone, size);
		if (zone == NULL)
			return (NULL);
		else
			block = block_init_zone(zone, size);
	}
	return (block + sizeof(*block));
}
