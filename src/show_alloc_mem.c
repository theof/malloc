/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useless_functs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 19:13:30 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 13:09:31 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

extern t_allocs	g_allocs[3];

size_t	get_n_zones(void)
{
	unsigned	type;
	size_t		count;
	t_zone		*link;
	t_zone		*head;

	type = E_ALLOC_TINY;
	count = 0;
	while (type <= E_ALLOC_LARGE)
	{
		head = g_allocs[type].zones;
		if (head != NULL)
		{
			link = head;
			while (1)
			{
				count++;
				link = link->next;
				if (link == head)
					break;
			}
		}
		type++;
	}
	return (count);
}

void	array_fill(t_alloc_zone *array)
{
	unsigned	type;
	t_zone		*link;
	t_zone		*head;

	type = E_ALLOC_TINY;
	while (type <= E_ALLOC_LARGE)
	{
		head = g_allocs[type].zones;
		if (head != NULL)
		{
			link = head;
			while (1)
			{
				array->zone = link;
				array->alloc_type = type;
				array++;
				link = link->next;
				if (link == head)
					break;
			}
		}
		type++;
	}
	array->alloc_type = E_ALLOC_NONE;
}

void	array_awesome_sort(t_alloc_zone *array)
{
	t_alloc_zone	tmp;
	size_t			n;
	size_t			o;

	n = 0;
	while (array[n].alloc_type != E_ALLOC_NONE)
	{
		o = n + 1;
		while (array[o].alloc_type != E_ALLOC_NONE)
		{
			if (array[n].zone > array[o].zone)
			{
				tmp = array[n];
				array[n] = array[o];
				array[o] = tmp;
			}
			o++;
		}
		n++;
	}
}

size_t	print_block(t_block *block)
{
	size_t	size;

	size = 0;
	if (!block->flags.available)
	{
		size = BLOCK_SIZE(block->size) - 2 * sizeof(t_block);
		ft_puthex((size_t)(block + 1));
		ft_putstr(" - ");
		ft_puthex((size_t)((char*)(block + 1) + size));
		ft_putstr(" : ");
		ft_putnbr(size); /* FIXME overflows int */
		ft_putstr(" bytes\n");
	}
	if (!block->flags.bound_right)
	{
		block = get_next_block(block);
		if (block != NULL)
			size += print_block(block);
	}
	return (size);
}

void	print_zone_header(t_alloc_zone alloc)
{
	char	*strings[] = {
		"TINY",
		"SMALL",
		"LARGE"
	};

	ft_putstr(strings[alloc.alloc_type]);
	ft_putstr(" : ");
	ft_puthex((size_t)alloc.zone);
	ft_putchar(10);
}

void	show_alloc_mem(void)
{
	size_t			size;
	t_alloc_zone	*array;
	size_t			total;
	size_t			i;

	size = align_size((get_n_zones() + 1) * sizeof(*array), getpagesize());
	array = mmap(0, size, PROT_READ_WRITE, MAP_ANON_PRIVATE, -1, 0);
	if (array == NULL)
		return ;
	array_fill(array);
	array_awesome_sort(array);
	total = 0;
	i = 0;
	while (array[i].alloc_type != E_ALLOC_NONE)
	{
		print_zone_header(array[i]);
		total += print_block((t_block*)(array[i].zone + 1));
		i++;
	}
	ft_putstr("Total : ");
	ft_putnbr(total); // Fixme overflows int
	ft_putstr(" bytes\n");
	munmap(array, size);
}
