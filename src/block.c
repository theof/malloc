/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 03:21:47 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/11 19:47:20 by tvallee          ###   ########.fr       */
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

t_block			*get_prev_block(t_block const *block)
{
	size_t	prev_size;

	if (block->flags.bound_left == TRUE)
		return (NULL);
	else
	{
		prev_size = BLOCK_SIZE((block - 1)->size);
		return ((t_block*)((char*)block - prev_size));
	}
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
		ft_puthex((size_t)current);
		ft_putchar(10);
		count++;
		current = current->next;
		if (current == head)
			break;
	}
	return (count);
}

#define DEBUG(x, y) ft_putstr(x " value: ");ft_puthex((size_t)y);ft_putendl("");
t_block_free	*block_push_free_list(t_block *block, unsigned type)
{
	t_block_free	*current;
	t_block_free	**head;

	ft_putstr("push");
	head = &(g_allocs[type].free_blocks);
	current = (t_block_free*)block;
	if (*head != NULL)
	{
		DEBUG("current", current);
		DEBUG("current->size", current->header.size);
		DEBUG("*head", *head);
		DEBUG("*head->prev", (*head)->prev);
		DEBUG("*head->prev->next", (*head)->prev->next);
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
	ft_putstr(" ok");
	return ((t_block_free*)current);
}

t_block_free	*block_replace_free_list(t_block_free *old, t_block *new, unsigned type)
{
	t_block_free	*replacement;

	replacement = (t_block_free*)new;
	if (old->next == old)
	{
		replacement->next = replacement;
		replacement->prev = replacement;
	}
	else
	{
		replacement->next = old->next;
		replacement->prev = old->prev;
		old->prev->next = replacement;
		old->next->prev = replacement;
	}
	g_allocs[type].free_blocks = replacement;
	return (replacement);
}

t_block			*block_pop_free_list(t_block_free *block, unsigned type)
{
	ft_putstr("pop");
	if (block->next == block)
	{
		g_allocs[type].free_blocks = NULL;
	}
	else
	{
		block->prev->next = block->next;
		block->next->prev = block->prev;
		g_allocs[type].free_blocks = block->next;
	}
	ft_putstr(" ok");
	return ((t_block*)block);
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

/* block edit */

t_block			*block_shrink(t_block *block, size_t size, unsigned type) {
	t_block	*next;
	t_block	old;
	size_t	extra_space;

	extra_space = BLOCK_SIZE(block->size) - size;
	if (allocs_assert_available_block_type(extra_space, type))
	{
		next = get_next_block(block);
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
	next = get_next_block(block);
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
	
	footer = get_next_block(block) - 1;
	footer->size = block->size;
}

/* block coalesce */

t_block_free	*coalesce_right(t_block *current, t_block_free *next, unsigned type)
{
	current->size += BLOCK_SIZE(next->header.size);
	current->flags.bound_right = next->header.flags.bound_right;
	current->flags.available = TRUE;
	block_update_footer(current);
	return (block_replace_free_list(next, current, type));
}

t_block_free	*coalesce_left(t_block *current, t_block_free *prev)
{
	prev->header.size += BLOCK_SIZE(current->size);
	prev->header.flags.bound_right = current->flags.bound_right;
	block_update_footer((t_block*)prev);
	return (prev);
}

t_block_free	*coalesce_left_right(t_block *current, t_block_free *prev, t_block_free *next, unsigned type)
{
	block_pop_free_list(next, type);
	prev->header.size += BLOCK_SIZE(next->header.size) + BLOCK_SIZE(current->size);
	prev->header.flags.bound_right = next->header.flags.bound_right;
	block_update_footer((t_block*)prev);
	return (prev);
}

t_block_free	*coalesce(t_block *current, unsigned type)
{
	t_block	*prev;
	t_block	*next;

	prev = get_prev_block(current);
	next = get_next_block(current);
	if (prev != NULL && prev->flags.available == TRUE)
	{
		if (next != NULL && next->flags.available == TRUE)
			return (coalesce_left_right(current, (t_block_free*)prev, (t_block_free*)next, type));
		else
			return (coalesce_left(current, (t_block_free*)prev));
	}
	else
	{
		if (next != NULL && next->flags.available == TRUE)
			return (coalesce_right(current, (t_block_free*)next, type));
		else
		{
			current->flags.available = TRUE;
			block_update_footer(current);
			return (block_push_free_list(current, type));
		}
	}
}
