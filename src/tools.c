/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 11:35:03 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/27 11:35:24 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

size_t	align_size(size_t size, size_t alignment)
{
	size_t	truncated;

	truncated = size & (~(alignment - 1));
	if (truncated < size)
		return (truncated + alignment);
	else
		return (truncated);
}
