/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/05 16:42:41 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/23 05:55:50 by tvallee          ###   ########.fr       */
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
# define TINY_ZONE_SIZE (32 * getpagesize())
# define SMALL_SIZE
# define SMALL_ZONE_SIZE (320 * getpagesize())

# define MAGIC_STATIC_ZONE 0

# define PROT_READ_WRITE PROT_READ | PROT_WRITE
# define MAP_ANON_PRIVATE MAP_ANON | MAP_PRIVATE

typedef struct	s_block
{
	size_t	size;
	char	available;
}				t_block;

typedef struct	s_free_block
{
	size_t				size;
	struct s_free_block	*next;
	struct s_free_block	*prev;
}				t_free_block;

typedef struct	s_zone
{
	struct s_zone	*next;
	struct s_zone	*prev;
	struct s_block	block;
}				t_zone;

enum	e_zone_type
{
	E_ZONE_TINY = 0,
	E_ZONE_SMALL,
	E_ZONE_LARGE
};

void			free(void *ptr);
void			*malloc(size_t size);
void			*realloc(void *ptr, size_t size);
void			*calloc(size_t numitems, size_t size);
void			show_alloc_mem();

#endif
