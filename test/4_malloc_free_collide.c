/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_malloc_free_collide.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 15:32:25 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 17:52:06 by tvallee          ###   ########.fr       */
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

	printf("TINY_SIZE: %zu\n", TINY_SIZE);
	printf("TINY_BLOCK_SIZE: %zu\n", TINY_BLOCK_SIZE);
	printf("TINY_ZONE_SIZE: %zu\n", TINY_ZONE_SIZE);
	printf("SMALL_SIZE: %zu\n", SMALL_SIZE);
	printf("SMALL_BLOCK_SIZE: %zu\n", SMALL_BLOCK_SIZE);
	printf("SMALL_ZONE_SIZE: %zu\n", SMALL_ZONE_SIZE);
	
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
