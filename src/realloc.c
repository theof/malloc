/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 18:03:45 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/06 20:11:41 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <unistd.h>
#include "libft.h"

/*
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
*/

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
	/*
	size = block_size(size);
	type = common_alloc_type(BLOCK_SIZE(block->size), size);
	if (type != E_ALLOC_NONE && 0)
	{
		if (size < BLOCK_SIZE(block->size))
		{
			return block_shrink(block, size, type);
		}
		if ((diff = size - BLOCK_SIZE(block->size)) == 0)
		{
			return (ptr);
		}
		resized = block_enlarge(block, diff, type);
		if (resized != NULL)
		{
			return (ptr);
		}
	}*/
	new = malloc(size);
	if (new)
	{
		ft_memcpy(new, ptr, BLOCK_SIZE(block->size) - 2 * sizeof(t_block));
		free(ptr);
	}
	return (new);
}
