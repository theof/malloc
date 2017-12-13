/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 03:21:47 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/13 18:21:22 by tvallee          ###   ########.fr       */
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

size_t	get_list_size(t_block_free* head)
{
	t_block_free	*current;
	size_t			count;

	current = head;
	count = 0;
	if (current == NULL)
		return (count);
	while (1)
	{
		count++;
		current = current->next;
		if (current == head)
			break;
	}
	return (count);
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
	ft_putstr("block_fit");
	while (1)
	{
		if (size <= BLOCK_SIZE(current->header.size))
		{
			ft_putstr("/block_fit");
			return (block_create(current, size, type));
		}
		if (current->next == *head)
			break;
		current = current->next;
		ft_puthex((size_t)(current));
		ft_putendl(" current");
	}
	return (NULL);
}

t_block			*block_create(t_block_free *available, size_t size, unsigned type)
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

	ft_putstr("\t\tiniting block zone at ");
	request = (t_block*)(zone + 1);
	ft_puthex((size_t)request);
	request->size = zone_size - sizeof(t_zone);
	ft_putstr(" size: ");
	ft_putnbr(request->size);
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
