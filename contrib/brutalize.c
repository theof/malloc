/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brutalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 18:05:32 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/18 12:51:19 by tvallee          ###   ########.fr       */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/malloc.h"
#include "../libft/include/libft.h"

typedef struct	s_data
{
	void		*ptr;
	uint32_t	size;
}				t_data;

static uint32_t	get_random_size(unsigned *type)
{
	uint32_t	ret;

	*type = arc4random_uniform(3);
	switch (*type)
	{
		case 0:
			ret = arc4random_uniform(TINY_SIZE + 1);
			break;
		case 1:
			do {
				ret = arc4random_uniform(SMALL_SIZE + 1);
			} while (ret <= TINY_SIZE);
			break;
		case 2:
			do {
				ret = arc4random_uniform(1024 * 1024 * 42);
			} while (ret <= SMALL_SIZE);
			break;
	}
	return (ret);
}

int main()
{
	t_data		table[200] = { {.ptr = NULL, .size = 0 } };
	t_data		*current;
	unsigned	type;
	char		type_map[3][6] = {"TINY", "SMALL", "LARGE"};

	write(1, "=========BEGIN BRUTALIZE=========\n", sizeof("=========BEGIN BRUTALIZE========="));
	while (1)
	{
		ft_putstr("\n=> new iteration: ");

		current = &(table[arc4random_uniform(200)]);
		if (current->ptr == NULL)
		{
			current->size = get_random_size(&type);

			ft_putstr("malloc ");
			ft_putstr(type_map[type]);
			ft_putstr(" size ");
			ft_putnbr(current->size);
			ft_putchar(10);

			current->ptr = malloc(current->size);
			if (current->ptr == NULL)
			{
				ft_putendl("malloc returned NULL !");
				return (1);
			}
		}
		else {
			switch (arc4random_uniform(5))
			{
				case 0:
					ft_putendl("free");
					free(current->ptr);
					current->ptr = NULL;
					break;
				case 1:
					ft_putstr("realloc size: ");
					ft_putnbr(current->size);
					ft_putstr(" => ");
					current->size = get_random_size(&type);
					ft_putnbr(current->size);
					ft_putchar(10);
					current->ptr = realloc(current->ptr, current->size);
					break;
				default:
					ft_putendl("no op");
					break;
			}
		}
		if (current->ptr != NULL)
		{
			ft_putstr("=> filling available memory: ");
			arc4random_buf(current->ptr, current->size);
			ft_putendl("OK !");
		}
	}
}
