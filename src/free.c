/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 19:14:09 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 13:57:55 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs g_allocs[3];

static t_block	*coalesce_right(t_block *current, unsigned type)
{
	t_block_free	*next;
	t_block			old;

	next = (t_block_free*)get_next_block(current);
	block_pop_free_list(next, type);
	old = *current;
	current->size = BLOCK_SIZE(old.size) + BLOCK_SIZE(next->header.size);
	current->flags.available = TRUE;
	current->flags.bound_left = old.flags.bound_left;
	current->flags.bound_right = next->header.flags.bound_right;
	block_copy_footer(current);
	block_push_free_list(current, type);
	return (current);
}

static t_block	*coalesce_left(t_block *current, unsigned type)
{
	t_block	*prev;
	size_t	prev_size;
	
	prev_size = BLOCK_SIZE((current - 1)->size);
	prev = (t_block*)((char*)current - prev_size);
	return (coalesce_right(prev, type));
}

static t_block	*coalesce(t_block *current, int prev_free,
		int next_free, unsigned type)
{
	if (prev_free == TRUE)
	{
		if (next_free == TRUE)
			return (coalesce_left(coalesce_right(current, type), type));
		else
			return (coalesce_left(current, type));
	}
	else
	{
		if (next_free == TRUE)
			return (coalesce_right(current, type));
		else
		{
			current->flags.available = TRUE;
			block_copy_footer(current);
			block_push_free_list(current, type);
			//ft_putnbr(current->size);
			//ft_putendl(" current->size");
			return (current);
		}
	}
}

void	free(void *ptr)
{
	int			prev_free;
	int			next_free;
	t_block		*current;
	unsigned	type;
	
	//ft_putchar(10);
	//ft_putendl("free: ");
	if (ptr == NULL || allocs_is_ours(ptr) == FALSE)
		return ;
	current = (t_block*)((char*)ptr - sizeof(t_block));
	type = allocs_get_type(BLOCK_SIZE(current->size) - 2 * sizeof(t_block));
	if (current->flags.bound_left)
		prev_free = FALSE;
	else
		prev_free = (current - 1)->flags.bound_left;
	if (current->flags.bound_right)
		next_free = FALSE;
	else
		next_free = get_next_block(current)->flags.bound_right;
	current = coalesce(current, prev_free, next_free, type);
	//ft_putstr("freed block: ");
	//ft_puthex((size_t)(current));
	//ft_putchar(10);
	if (current->flags.bound_left && current->flags.bound_right)
	{
		zone_unmap((t_zone*)((char*)current - sizeof(t_zone)));
		//ft_putstr("removing zone: ");
		//ft_puthex((size_t)((char*)current - sizeof(t_zone)));
		//ft_putstr(" type: ");
		//ft_putnbr(type);
		//ft_putchar(10);
	}
	return ;
}
