/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 18:03:45 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/18 14:30:49 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <unistd.h>
#include "libft.h"

static void		*try_resize(t_block *block, size_t new_size, unsigned type)
{
	size_t	diff;
	t_block	*resized;

	if (new_size < BLOCK_SIZE(block->size))
		return (block_shrink(block, new_size, type));
	if ((diff = new_size - BLOCK_SIZE(block->size)) == 0)
		return (block);
	resized = block_enlarge(block, diff, type);
	if (resized != NULL)
		return (block);
	return (NULL);
}

void			*realloc(void *ptr, size_t size)
{
	t_block		*block;
	t_block		*resized;
	void		*new;
	unsigned	type;
	size_t		size_block;

	if (ptr == NULL)
		return (malloc(size));
	if ((type = allocs_is_ours(ptr)) == E_ALLOC_NONE)
		return (NULL);
	block = (t_block*)ptr - 1;
	size_block = block_size(size);
	if (type == allocs_get_type_block(size_block))
	{
		if ((resized = try_resize(block, size_block, type)) != NULL)
			return (resized + 1);
	}
	if ((new = malloc(size)) != NULL)
	{
		if (size > BLOCK_SIZE(block->size) - 2 * sizeof(t_block))
			ft_memcpy(new, ptr, BLOCK_SIZE(block->size) - 2 * sizeof(t_block));
		else
			ft_memcpy(new, ptr, size);
		free(ptr);
	}
	return (new);
}
