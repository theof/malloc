/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_resize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 15:41:18 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/15 19:23:50 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

t_block			*block_shrink(t_block *block, size_t size, unsigned type)
{
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

t_block			*block_enlarge(t_block *block, size_t diff, unsigned type)
{
	t_block	*next;

	if ((next = get_next_block(block)) == NULL)
		return (NULL);
	if (next->flags.available == TRUE && BLOCK_SIZE(next->size) <= diff)
	{
		block_create((t_block_free*)next, diff, type);
		block->size += diff;
		block_update_footer(block);
		return (block);
	}
	return (NULL);
}
