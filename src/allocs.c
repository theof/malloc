/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 08:14:23 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/27 22:47:18 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_allocs	g_allocs[3] = {
	[E_ALLOC_TINY] = {
		.zones = NULL,
		.free_blocks = NULL,
	},
	[E_ALLOC_SMALL] = {
		.zones = NULL,
		.free_blocks = NULL,
	},
	[E_ALLOC_LARGE] = {
		.zones = NULL,
		.free_blocks = NULL,
	},
};

unsigned	allocs_get_type(size_t request_size)
{
	if (request_size > SMALL_SIZE)
		return (E_ALLOC_LARGE);
	else if (request_size > TINY_SIZE)
		return (E_ALLOC_SMALL);
	else
		return (E_ALLOC_TINY);
}

unsigned	allocs_get_type_by_zone_size(size_t zone_size)
{
	if (zone_size == TINY_ZONE_SIZE)
		return (E_ALLOC_TINY);
	else if (zone_size == SMALL_ZONE_SIZE)
		return (E_ALLOC_SMALL);
	else
		return (E_ALLOC_LARGE);
}
