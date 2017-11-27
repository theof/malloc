/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/22 22:07:52 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/27 22:01:22 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs	g_allocs[3];

static void	insert_zone(t_zone *head, t_zone *new)
{
	if (head != NULL)
	{
		new->prev = head->prev;
		new->next = head;
		head->prev->next = new;
		head->prev = new;
	}
	else
	{
		new->prev = new;
		new->next = new;
	}
}

size_t		zone_map(t_zone **dst, size_t size, unsigned type)
{
	t_zone	*new;
	t_zone	**head;
	size_t	zone_size;

	if (type == E_ALLOC_LARGE)
		zone_size = align_size(sizeof(t_zone) + size, getpagesize());
	else if (type == E_ALLOC_SMALL)
		zone_size = SMALL_ZONE_SIZE;
	else
		zone_size = TINY_ZONE_SIZE;
	new = mmap(0, zone_size, PROT_READ_WRITE, MAP_ANON_PRIVATE, -1, 0);
	head = &(g_allocs[type].zones);
	if (new != NULL)
		insert_zone(*head, new);
	*dst = new;
	*head = new;
	return (zone_size);
}
