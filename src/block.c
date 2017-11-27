/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 03:21:47 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/27 22:31:38 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

extern t_zone	g_zones[3];

size_t			block_size(size_t size)
{
	if (size < (BLOCK_MIN_SIZE))
		return (BLOCK_MIN_SIZE);
	else
		return (align_size(size + sizeof(t_block) * 2, 0x8));
}

t_block	*get_next_block(t_block const *block)
{
	if (block->flags.bound_right)
		return (NULL);
	else
		return ((t_block*)((char*)block + BLOCK_SIZE(block->size)));
}

t_block	*block_init_zone(t_zone *zone, size_t zone_size, size_t size)
{
	t_block	*request;
	t_block	*remainder;

	request = (t_block*)(zone + 1);
	request->size = size;
	request->flags.available = FALSE;
	request->flags.bound_left = TRUE;
	((t_block*)((char*)request + size - sizeof(t_block)))->size = request->size;
	remainder = ((t_block*)((char*)request + size));
	remainder->size = zone_size - size;
	remainder->flags.available = TRUE;
	remainder->flags.bound_right = TRUE;
	return (request);
}

/*
t_block			*block_fit(size_t size)
{
	
}

t_block			*block_init_zone(t_zone *zone, size_t size)
{
	
}
*/
