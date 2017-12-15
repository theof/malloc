/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 18:03:45 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/15 21:56:51 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <unistd.h>
#include "libft.h"

void			*realloc(void *ptr, size_t size)
{
	size_t		diff;
	t_block		*block;
	t_block		*resized;
	void		*new;
	unsigned	type;
	size_t		size_block;

	if (ptr == NULL)
		return (malloc(size));
	if ((type = allocs_is_ours(ptr)) == E_ALLOC_NONE)
		return (NULL);
	ft_putstr("realloc(");
	ft_puthex((size_t)ptr);
	ft_putstr(", ");
	block = (t_block*)ptr - 1;
	size_block = block_size(size);
	ft_putnbr(size_block);
	ft_putstr("): old size ");
	ft_putnbr(BLOCK_SIZE(block->size));
	ft_putchar(10);
	if (type == allocs_get_type_block(size_block))
	{
		if (size_block < BLOCK_SIZE(block->size))
			return block_shrink(block, size_block, type);
		if ((diff = size_block - BLOCK_SIZE(block->size)) == 0)
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
