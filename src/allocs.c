/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 08:14:23 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 13:24:10 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

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

unsigned	allocs_get_type(size_t request_size)
{
	if (request_size > SMALL_SIZE)
		return (E_ALLOC_LARGE);
	else if (request_size > TINY_SIZE)
		return (E_ALLOC_SMALL);
	else
		return (E_ALLOC_TINY);
}

unsigned	allocs_get_type_by_zone_size(size_t zone_size)
{
	if (zone_size == TINY_ZONE_SIZE)
		return (E_ALLOC_TINY);
	else if (zone_size == SMALL_ZONE_SIZE)
		return (E_ALLOC_SMALL);
	else
		return (E_ALLOC_LARGE);
}

int			allocs_assert_block_size_type(size_t size, unsigned type)
{
	if (size < BLOCK_MIN_SIZE)
		return (FALSE);
	if (allocs_get_type(size) < type)
		return (FALSE);
	return (TRUE);
}

int		allocs_is_ours(void *ptr)
{
	t_block		*block;
	unsigned	type;
	t_zone		*head;
	t_zone		*current_zone;
	t_block		*current_block;
	
	block = (t_block*)ptr - 1;
	type = 0;
	while (type < E_ALLOC_NONE)
	{
		if ((head = g_allocs[type].zones) != NULL)
		{
			current_zone = head;
			while (1)
			{
				current_block = (t_block*)(current_zone + 1);
				while (current_block != NULL)
				{
					if (current_block == block)
						return (TRUE);
					current_block = get_next_block(current_block);
				}
				current_zone = current_zone->next;
				if (current_zone == head)
					break ;
			}
		}
		type++;
	}
	return (FALSE);
}
