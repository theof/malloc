/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_malloc_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 15:24:19 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 16:06:59 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#define M (1024 * 1024)

void	*malloc_scribble(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	memset(ptr, 42, size);
	return (ptr);
}

int main()
{
	int	i;
	int	*array;
	int	sizes[] = {42, 0, 84, 42 * M, 84 * M};

	array = malloc_scribble(42 * sizeof(*sizes));
	for (i = 0; i < 42; i++)
		array[i] = 42;
	return (0);
}
