/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 03:21:47 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/07 19:44:51 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs	g_allocs[3];

/* block misc */

size_t			block_size(size_t size)
{
	size_t	aligned;

	aligned = align_size(size + sizeof(t_block) * 2, 0x8);
	if (aligned < (BLOCK_MIN_SIZE))
		return (BLOCK_MIN_SIZE);
	else
		return (aligned);
}

t_block			*get_next_block(t_block const *block)
{
	if (block->flags.bound_right)
		return (NULL);
	else
		return ((t_block*)((char*)block + BLOCK_SIZE(block->size)));
}

#define DEBUG(x, y) ft_putstr(x " value: ");ft_puthex((size_t)y);ft_putendl("");
void			block_push_free_list(t_block *block, unsigned type)
{
	t_block_free	*current;
	t_block_free	**head;

	ft_putstr("block_push");
	head = &(g_allocs[type].free_blocks);
	current = (t_block_free*)block;
	if (*head != NULL)
	{
		DEBUG("current", current);
		DEBUG("(*head)", (*head));
		DEBUG("(*head)->prev", (*head)->prev);
		current->prev = (*head)->prev;
		current->next = (*head);
		(*head)->prev->next = current;
		(*head)->prev = current;
	}
	else
	{
		current->next = current;
		current->prev = current;
	}
	*head = current;
	ft_putstr("/block_push ");
}


void			block_pop_free_list(t_block_free *block, unsigned type)
{
	t_block_free	**head;

	ft_putstr("block_pop");
	head = &(g_allocs[type].free_blocks);
	if (block->next == block)
	{
		*head = NULL;
	}
	else
	{
		DEBUG("block", block);
		DEBUG("block->prev", block->prev);
		DEBUG("block->next", block->next);
		DEBUG("block->prev->next", block->prev->next);
		DEBUG("block->next->prev", block->next->prev);
		DEBUG("*head", *head);
		block->prev->next = block->next;
		block->next->prev = block->prev;
		*head = block->prev;
	}
	block->next = NULL;
	block->prev = NULL;
	ft_putstr("/block_pop ");
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
		{
			*head = current;
			return (block_create(current, size, type));
		}
		if (current->next == *head)
			break;
		current = current->next;
	}
	return (NULL);
}

/* block edit */

t_block			*block_shrink(t_block *block, size_t size, unsigned type)
{
	t_block	*next;
	t_block	old;
	size_t	extra_space;

	extra_space = BLOCK_SIZE(block->size) - size;
	if (allocs_assert_available_block_type(extra_space, type))
	{
		next = (t_block*)((char*)block + size);
		next->size = extra_space;
		next->flags.bound_left = FALSE;
		next->flags.bound_right = block->flags.bound_right;
		next->flags.available = FALSE;
		block_update_footer(next);
		old = *block;
		block->size = size;
		block->flags.bound_left = old.flags.bound_left;
		block->flags.available = FALSE;
		block_update_footer(next);
		free(next);
	}
	return (block);
}

t_block			*block_enlarge(t_block *block, size_t diff, unsigned type) {
	t_block	*next;

	if (block->flags.bound_right == TRUE)
		return (NULL);
	next = (t_block*)((char*)block + BLOCK_SIZE(block->size));
	if (next->flags.available == TRUE && BLOCK_SIZE(next->size) <= diff)
	{
		block_create((t_block_free*)next, diff, type);
		block->size += diff;
		block_update_footer(block);
		return (block);
	}
	return (NULL);
}

t_block			*block_create(t_block_free *available, size_t size, unsigned type)
{
	t_block			*new;
	t_block			*remaining;
	size_t			extra_space;
	t_block			old;
	
	extra_space = BLOCK_SIZE(available->header.size) - size;
	if (allocs_assert_available_block_type(extra_space, type))
	{
		old = *((t_block*)available);
		block_pop_free_list(available, type);
		new = (t_block*)available;
		new->size = size;
		new->flags.bound_left = old.flags.bound_left;
		block_update_footer(new);
		remaining = (t_block*)((char*)available + size);
		remaining->size = extra_space;
		remaining->flags.bound_right = old.flags.bound_right;
		remaining->flags.available = TRUE;
		block_update_footer(remaining);
		block_push_free_list(remaining, type);
	}
	else
	{
		new = (t_block*)available;
		available->header.flags.available = FALSE;
		block_update_footer((t_block*)available);
		block_pop_free_list(available, type);
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
	block_push_free_list(request, type);
	return ((t_block_free*)request);
}

void			block_update_footer(t_block *block)
{
	t_block	*footer;
	
	footer = (t_block*)((char*)block + BLOCK_SIZE(block->size) - sizeof(t_block));
	footer->size = block->size;
}

/* block coalesce */

t_block			*coalesce_right(t_block *current, unsigned type)
{
	t_block_free	*next;
	t_block			old;

	next = (t_block_free*)get_next_block(current);
	block_pop_free_list(next, type);
	old = *current;
	current->size = BLOCK_SIZE(old.size) + BLOCK_SIZE(next->header.size);
	current->flags.available = TRUE;
	current->flags.bound_left = old.flags.bound_left;
	current->flags.bound_right = next->header.flags.bound_right;
	block_update_footer(current);
	block_push_free_list(current, type);
	return (current);
}

t_block			*coalesce_left(t_block *current, unsigned type)
{
	t_block	*prev;
	size_t	prev_size;
	
	current->flags.available = TRUE;
	block_update_footer(current);
	block_push_free_list(current, type);
	prev_size = BLOCK_SIZE((current - 1)->size);
	prev = (t_block*)((char*)current - prev_size);
	return (coalesce_right(prev, type));
}

t_block			*coalesce(t_block *current, int prev_free,
		int next_free, unsigned type)
{
	if (prev_free == TRUE)
	{
		if (next_free == TRUE)
			return (coalesce_left(coalesce_right(current, type), type));
		else
			return (coalesce_left(current, type));
	}
	else
	{
		if (next_free == TRUE)
			return (coalesce_right(current, type));
		else
		{
			current->flags.available = TRUE;
			block_update_footer(current);
			block_push_free_list(current, type);
			return (current);
		}
	}
}
