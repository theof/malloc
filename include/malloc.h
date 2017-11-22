/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/05 16:42:41 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/22 03:46:46 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/resource.h>

/*
** n, N, m, M
*/

# define TINY_SIZE
# define TINY_ZONE_SIZE
# define SMALL_SIZE
# define SMALL_ZONE_SIZE

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t numitems, size_t size);
void	show_alloc_mem();

#endif
