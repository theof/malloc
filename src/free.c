/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 19:14:09 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/08 17:28:33 by tvallee          ###   ########.fr       */
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
	int			prev_free;
	int			next_free;
	t_block		*current;
	unsigned	type;
	
	ft_putstr("free(");
	ft_puthex((size_t)ptr - sizeof(t_block));
	ft_putstr("): ");
	if (ptr == NULL || allocs_is_ours(ptr) == FALSE)
	{
		ft_putendl("not ours");
		return ;
	}
	current = (t_block*)ptr - 1;
	type = allocs_get_type_block(BLOCK_SIZE(current->size));
	if (current->flags.bound_left)
		prev_free = FALSE;
	else
		prev_free = (current - 1)->flags.available;
	if (current->flags.bound_right)
		next_free = FALSE;
	else
		next_free = get_next_block(current)->flags.available;
	current = coalesce(current, prev_free, next_free, type);
	if (current->flags.bound_left && current->flags.bound_right)
	{
		block_pop_free_list((t_block_free*)current, type);
		zone_unmap((t_zone*)current - 1);
	}
	else
	{
		if (type == 0)
		{
			ft_putchar(10);
			hexdump(current);
		}
	}
	ft_putendl(" OK !");
	return ;
}
