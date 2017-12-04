/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 03:21:47 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/04 12:08:57 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs	g_allocs[3];

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

void	block_push_free_list(t_block *block, unsigned type)
{
	t_block_free	*current;
	t_block_free	**head;
	
	head = &(g_allocs[type].free_blocks);
	current = (t_block_free*)block;
	if (*head != NULL)
	{
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
}

void	block_pop_free_list(t_block_free *block, unsigned type)
{
	t_block_free	**head;
	
	head = &(g_allocs[type].free_blocks);
	if (block->next == block)
	{
		*head = NULL;
	}
	else
	{
		block->prev->next = block->next;
		block->next->prev = block->prev;
		*head = block->prev;
	}
}

void	block_update_footer(t_block *block)
{
	t_block	*footer;
	
	footer = (t_block*)((char*)block + BLOCK_SIZE(block->size) - sizeof(t_block));
	footer->size = block->size;
}

t_block	*block_create(t_block_free *available, size_t size, unsigned type)
{
	t_block			*new;
	t_block			*remaining;
	size_t			extra_space;
	t_block			old;
	
	extra_space = BLOCK_SIZE(available->header.size) - size;
	if (allocs_assert_block_size_type(extra_space, type))
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
		block_pop_free_list(available, type);
		block_update_footer((t_block*)available);
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
