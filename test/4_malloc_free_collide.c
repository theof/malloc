/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_malloc_free_collide.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 15:32:25 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 16:24:22 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "../include/malloc.h"

void	*malloc_scribble(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	memset(ptr, 42, size);
	return (ptr);
}

void	get_space(char **ptr)
{
	*ptr++ = malloc_scribble(TINY_SIZE);
	*ptr++ = malloc_scribble(TINY_SIZE);
	*ptr++ = malloc_scribble(TINY_SIZE);
}

int main()
{
	char	*ptr[3];
	char	*large;

	write(1, "begin test\n", 11);
	get_space(ptr);
	free(ptr[0]);
	free(ptr[1]);
	free(ptr[2]);

	get_space(ptr);
	free(ptr[0]);
	free(ptr[2]);
	free(ptr[1]);

	get_space(ptr);
	free(ptr[1]);
	free(ptr[0]);
	free(ptr[2]);

	get_space(ptr);
	free(ptr[1]);
	free(ptr[2]);
	free(ptr[0]);

	get_space(ptr);
	free(ptr[2]);
	free(ptr[1]);
	free(ptr[0]);

	get_space(ptr);
	free(ptr[2]);
	free(ptr[0]);
	free(ptr[1]);

	large = malloc_scribble(SMALL_SIZE + 1);
	free(large);
}
