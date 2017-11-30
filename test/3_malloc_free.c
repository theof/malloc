/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_malloc_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 15:29:36 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/30 16:07:09 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

void	*malloc_scribble(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	memset(ptr, 42, size);
	return (ptr);
}

int main()
{
	int		i;
	char	*ptr;

	free(NULL);
	ptr = malloc(42);
	free(ptr);
	ptr = malloc(0);
	free(ptr);
	return (0);
}
