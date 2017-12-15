/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 19:14:09 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/15 20:28:42 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs g_allocs[3];

void	puthex(unsigned char byte)
{
	char	map[] = "0123456789ABCDEF";

	ft_putchar(map[byte >> 4]);
	ft_putchar(map[byte & 0xF]);
}

size_t	my_align_size(size_t size, size_t alignment)
{
	size_t	truncated;

	truncated = size & (~(alignment - 1));
	if (truncated < size)
	{
		return (truncated + alignment);
	}
	else
		return (truncated);
}

size_t	my_block_size(size_t size)
{
	size_t	aligned;

	aligned = my_align_size(size + sizeof(t_block) * 2, 0x8);
	if (aligned < (BLOCK_MIN_SIZE))
		return (BLOCK_MIN_SIZE);
	else
		return (aligned);
}

void	hexdump(t_block* current)
{
	size_t	l_count;

	l_count = 0;
	for (size_t n = 0; n < BLOCK_SIZE(current->size); n++)
	{
		if (l_count == 10)
		{
			ft_putchar('\n');
			l_count = 0;
		}
		puthex((((unsigned char*)current)[n]));
		ft_putchar(' ');
		l_count++;
	}
	ft_putchar('\n');
}

void	free(void *ptr)
{
	t_block			*current;
	t_block_free	*freed;
	unsigned		type;
	
	ft_putstr("free(");
	ft_puthex((size_t)ptr - sizeof(t_block));
	ft_putendl("):");
	if (ptr == NULL || (type = allocs_is_ours(ptr)) == E_ALLOC_NONE)
	{
		ft_putendl("unregistered memory !");
		return ;
	}
	current = (t_block*)ptr - 1;
	freed = coalesce(current, type);
	if (freed->header.flags.bound_left && freed->header.flags.bound_right)
	{
		block_pop_free_list(freed, type);
		zone_unmap((t_zone*)freed - 1, type);
	}
	ft_putendl("OK!");
	return ;
}
