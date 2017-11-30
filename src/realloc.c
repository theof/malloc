/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 18:03:45 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 15:16:09 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <unistd.h>
#include "libft.h"

static t_block	*block_shrink(t_block *block, size_t size, unsigned type)
{
	t_block	*next;
	t_block	old;
	size_t	extra_space;

	extra_space = BLOCK_SIZE(block->size) - size;
	if (allocs_assert_block_size_type(extra_space, type))
	{
		next = (t_block*)((char*)block + size);
		next->size = extra_space;
		next->flags.bound_left = FALSE;
		next->flags.bound_right = block->flags.bound_right;
		next->flags.available = FALSE;
		block_copy_footer(next);
		old = *block;
		block->size = size;
		block->flags.bound_left = old.flags.bound_left;
		block->flags.available = FALSE;
		block_copy_footer(next);
		free(next);
	}
	return (block);
}

static t_block	*block_enlarge(t_block *block, size_t diff, unsigned type)
{
	t_block	*next;

	if (block->flags.bound_right == TRUE)
		return (NULL);
	next = (t_block*)((char*)block + BLOCK_SIZE(block->size));
	if (next->flags.available == TRUE && BLOCK_SIZE(next->size) <= diff)
	{
		block_create((t_block_free*)next, diff, type);
		block->size += diff;
		block_copy_footer(block);
		return (block);
	}
	return (NULL);
}

static unsigned	common_alloc_type(size_t size_f, size_t size_s)
{
	unsigned	type_f;
	unsigned	type_s;
	
	type_f = allocs_get_type(size_f);
	type_s = allocs_get_type(size_s);
	if (type_f == E_ALLOC_LARGE || type_f != type_s)
		return (E_ALLOC_NONE);
	return (type_f);
}

void			*realloc(void *ptr, size_t size)
{
	size_t		diff;
	t_block		*block;
	t_block		*resized;
	void		*new;
	unsigned	type;

	if (ptr == NULL)
		return (malloc(size));
	if (allocs_is_ours(ptr) == FALSE)
	{
		return (NULL);
	}
	block = (t_block*)ptr - 1;
	size = block_size(size);
	type = common_alloc_type(BLOCK_SIZE(block->size), size);
	if (type != E_ALLOC_NONE)
	{
		if (size < BLOCK_SIZE(block->size))
			return block_shrink(block, size, type);
		if ((diff = size - BLOCK_SIZE(block->size)) == 0)
			return (ptr);
		resized = block_enlarge(block, diff, type);
		if (resized != NULL)
			return (ptr);
	}
	new = malloc(size);
	if (new)
	{
		ft_memcpy(new, ptr, BLOCK_SIZE(block->size) - 2 * sizeof(t_block));
		free(ptr);
	}
	return (new);
}
