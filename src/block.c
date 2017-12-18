/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 03:21:47 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/18 17:03:55 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs	g_allocs[3];

size_t			block_size(size_t size)
{
	size_t	aligned;

	aligned = align_size(size + sizeof(t_block) * 2, 0x8);
	if (aligned < (BLOCK_MIN_SIZE))
		return (BLOCK_MIN_SIZE);
	else
		return (aligned);
}

t_block			*block_fit(size_t size, unsigned type)
{
	t_block_free	**head;
	t_block_free	*current;

	if (type == E_ALLOC_LARGE)
		return (NULL);
	head = &(g_allocs[type].free_blocks);
	if (*head == NULL)
		return (NULL);
	current = *head;
	while (1)
	{
		if (size <= BLOCK_SIZE(current->header.size))
			return (block_create(current, size, type));
		if (current->next == *head)
			break ;
		current = current->next;
	}
	return (NULL);
}

t_block			*block_create(t_block_free *available, size_t size,
		unsigned type)
{
	t_block			*new;
	t_block			*remaining;
	size_t			extra_space;
	t_block			old;

	new = block_pop_free_list(available, type);
	extra_space = BLOCK_SIZE(new->size) - size;
	if (allocs_assert_available_block_type(extra_space, type))
	{
		old = *new;
		new->size = size;
		new->flags.available = FALSE;
		new->flags.bound_left = old.flags.bound_left;
		block_update_footer(new);
		remaining = get_next_block(new);
		remaining->size = extra_space;
		remaining->flags.bound_right = old.flags.bound_right;
		remaining->flags.available = TRUE;
		block_update_footer(remaining);
		block_push_free_list(remaining, type);
	}
	else
	{
		new->flags.available = FALSE;
		block_update_footer(new);
	}
	return (new);
}

t_block_free	*block_init_zone(t_zone *zone, size_t zone_size, unsigned type)
{
	t_block	*request;

	request = (t_block*)(zone + 1);
	request->size = zone_size - sizeof(t_zone);
	request->flags.bound_right = TRUE;
	request->flags.bound_left = TRUE;
	request->flags.available = TRUE;
	block_update_footer(request);
	return (block_push_free_list(request, type));
}

void			block_update_footer(t_block *block)
{
	t_block	*footer;

	footer = (t_block*)((char*)block + BLOCK_SIZE(block->size)) - 1;
	footer->size = block->size;
}
