/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/22 22:07:52 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/13 16:28:55 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs	g_allocs[3];

static void	zone_push(t_zone *new, unsigned type)
{
	t_zone	**head;

	head = &(g_allocs[type].zones);
	if (*head != NULL)
	{
		new->prev = (*head)->prev;
		new->next = (*head);
		(*head)->prev->next = new;
		(*head)->prev = new;
	}
	else
	{
		new->prev = new;
		new->next = new;
	}
	*head = new;
}

static void	zone_pop(t_zone *zone, unsigned type)
{
	t_zone	**head;
	
	head = &(g_allocs[type].zones);
	if (zone->next == zone)
		*head = NULL;
	else
	{
		zone->prev->next = zone->next;
		zone->next->prev = zone->prev;
		*head = zone->prev;
	}
}

size_t		zone_map(t_zone **dst, size_t size, unsigned type)
{
	t_zone	*new;
	size_t	zone_size;

	if (type == E_ALLOC_LARGE)
		zone_size = align_size(sizeof(t_zone) + size, getpagesize());
	else if (type == E_ALLOC_SMALL)
		zone_size = SMALL_ZONE_SIZE;
	else
		zone_size = TINY_ZONE_SIZE;
	new = mmap(0, zone_size, PROT_READ_WRITE, MAP_ANON_PRIVATE, -1, 0);
	if (new != MAP_FAILED)
	{
		zone_push(new, type);
		*dst = new;
	}
	else
		*dst = NULL;
	return (zone_size);
}

void		zone_unmap(t_zone *zone)
{
	size_t		zone_size;
	unsigned	type;

	zone_size = sizeof(t_zone) + BLOCK_SIZE(((t_block*)(zone + 1))->size);
	type = allocs_get_type_zone(zone_size);
	zone_pop(zone, type);
	munmap(zone, zone_size);
}
