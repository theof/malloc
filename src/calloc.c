/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 17:59:16 by tvallee           #+#    #+#             */
/*   Updated: 2017/06/12 23:18:15 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "malloc.h"
#include <unistd.h>

/*
** might be improved in circumstances where the kernel binds fresh memory with
** huge chunks.
*/

void	*calloc(size_t numitems, size_t size)
{
	void	*tmp;
	size_t	fsize;

	fsize = numitems * size;
	if ((tmp = malloc(fsize)))
		return (ft_memset(tmp, 0, fsize));
	return (tmp);
}
