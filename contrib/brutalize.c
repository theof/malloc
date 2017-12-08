/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brutalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 18:05:32 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/08 18:05:48 by tvallee          ###   ########.fr       */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/malloc.h"
#include "../libft/include/libft.h"

int main()
{
	void		*table[200] = {NULL};
	void		**ptr;
	uint32_t	rand;
	unsigned	type;
	unsigned char	pattern[4] = {0xDE, 0xAD, 0xBE, 0xEF};

	write(1, "=========BEGIN BRUTALIZE=========\n", sizeof("=========BEGIN BRUTALIZE========="));
	while (1)
	{
		ft_putendl("=> new iteration");
		type = 0;//arc4random_uniform(3);
		if (type == 0)
		{
			rand = arc4random_uniform(TINY_SIZE + 1);
		}
		else if (type == 1)
		{
			do {
				rand = arc4random_uniform(SMALL_SIZE + 1);
			} while (rand <= TINY_SIZE);
		}
		else
		{
			do {
				rand = arc4random_uniform(1024 * 1024 * 42);
			} while (rand <= SMALL_SIZE);
		}

		ptr = &(table[arc4random_uniform(200)]);

		if (*ptr == NULL)
		{
			*ptr = malloc(rand);
		}
		/*else if (arc4random_uniform(2))
			*ptr = realloc(*ptr, rand);*/
		else
		{
			free(*ptr);
			*ptr = NULL;
		}
		if (*ptr != NULL)
		{
			memset_pattern4(*ptr, pattern, rand);//arc4random_buf(*ptr, rand);
		}
	}
}
