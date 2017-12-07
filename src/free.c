/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 19:14:09 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/07 20:03:10 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs g_allocs[3];

void	free(void *ptr)
{
	int			prev_free;
	int			next_free;
	t_block		*current;
	unsigned	type;
	
	ft_putstr("free(");
	ft_puthex((size_t)ptr);
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
	ft_putendl(" OK !");
	return ;
}
