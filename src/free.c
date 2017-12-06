/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 19:14:09 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/06 22:39:19 by tvallee          ###   ########.fr       */
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
	
	return ;
	if (ptr == NULL || allocs_is_ours(ptr) == FALSE)
	{
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
		zone_unmap((t_zone*)current - 1);
	}
	return ;
}
