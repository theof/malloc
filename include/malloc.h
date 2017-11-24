/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/05 16:42:41 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/24 08:30:22 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/resource.h>

# define TINY_SIZE ((TINY_ZONE_SIZE - sizeof(t_zone)) / 100 - sizeof(t_block))
# define TINY_ZONE_SIZE (32 * getpagesize())
# define SMALL_SIZE ((SMALL_ZONE_SIZE - sizeof(t_zone)) / 100 - sizeof(t_block))
# define SMALL_ZONE_SIZE (320 * getpagesize())

# define PROT_READ_WRITE (PROT_READ | PROT_WRITE)
# define MAP_ANON_PRIVATE (MAP_ANON | MAP_PRIVATE)

# define TRUE 1
# define FALSE 0

typedef union	u_block_meta
{
	struct {
		size_t	available : 1;
		size_t	bound_left : 1;
		size_t	bound_right : 1;
	}		flags;
	size_t	size;
}				t_block_meta;

typedef struct	s_block
{
	t_block_meta	header;
	char			payload[];
}				t_block;

/*
** content is also footed by a t_block_meta
*/

typedef struct	s_zone
{
	struct s_zone	*next;
	struct s_zone	*prev;
	t_block_meta	first_block[];
}				t_zone;

typedef struct	s_block_free
{
	t_block_meta		header;
	struct s_block_free	*next;
	struct s_block_free *prev;
}				t_block_free;

typedef struct	s_allocs
{
	t_zone			*zones;
	t_block_free	*free_blocks;
}				t_allocs;

typedef struct	s_alloc_size
{
	size_t	payload;
	size_t	zone;
}				t_alloc_size;

enum	e_allocs_sizes
{
	E_ALLOC_TINY = 0,
	E_ALLOC_SMALL,
	E_ALLOC_LARGE
};

void			free(void *ptr);
void			*malloc(size_t size);
void			*realloc(void *ptr, size_t size);
void			*calloc(size_t numitems, size_t size);
void			show_alloc_mem();

#endif
