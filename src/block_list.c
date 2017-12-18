/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 15:38:12 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/18 14:24:22 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs	g_allocs[3];

t_block_free	*block_push_free_list(t_block *block, unsigned type)
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
	return ((t_block_free*)current);
}

t_block_free	*block_replace_free_list(t_block_free *old, t_block *new,
		unsigned type)
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
	return ((t_block*)block);
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
