/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 19:14:09 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/18 14:23:58 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs g_allocs[3];

void	free(void *ptr)
{
	t_block			*current;
	t_block_free	*freed;
	unsigned		type;

	if (ptr == NULL || (type = allocs_is_ours(ptr)) == E_ALLOC_NONE)
		return ;
	current = (t_block*)ptr - 1;
	freed = coalesce(current, type);
	if (freed->header.flags.bound_left && freed->header.flags.bound_right)
	{
		block_pop_free_list(freed, type);
		zone_unmap((t_zone*)freed - 1, type);
	}
	return ;
}
