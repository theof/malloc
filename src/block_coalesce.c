/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_coalesce.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 15:40:05 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/18 12:25:53 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs	g_allocs[3];

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

t_block_free	*coalesce_none(t_block *current, unsigned type)
{
	current->flags.available = TRUE;
	block_update_footer(current);
	return (block_push_free_list(current, type));
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
			return (coalesce_none(current, type));
	}
}
