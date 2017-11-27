/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/05 16:42:41 by tvallee           #+#    #+#             */
/*   Updated: 2017/11/27 22:03:16 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/resource.h>

# define FLOOR_8(x) (x & (~0x7))
# define BLOCK_SIZE(x) FLOOR_8(x)

# define BLOCK_MIN_SIZE (sizeof(t_block_free) + sizeof(t_block))

# define TINY_SIZE (TINY_BLOCK_SIZE - 2 * sizeof(t_block))
# define TINY_BLOCK_SIZE FLOOR_8((TINY_ZONE_SIZE - sizeof(t_zone)) / 100)
# define TINY_ZONE_SIZE (32 * getpagesize())

# define SMALL_SIZE (SMALL_BLOCK_SIZE - 2 * sizeof(t_block))
# define SMALL_BLOCK_SIZE FLOOR_8((SMALL_ZONE_SIZE - sizeof(t_zone)) / 100)
# define SMALL_ZONE_SIZE (320 * getpagesize())

# define PROT_READ_WRITE (PROT_READ | PROT_WRITE)
# define MAP_ANON_PRIVATE (MAP_ANON | MAP_PRIVATE)

# define TRUE 1
# define FALSE 0

typedef union	u_block
{
	struct {
		size_t	available : 1;
		size_t	bound_left : 1;
		size_t	bound_right : 1;
	}		flags;
	size_t	size;
}				t_block;

typedef struct	s_block_free
{
	t_block				header;
	struct s_block_free	*next;
	struct s_block_free *prev;
}				t_block_free;

typedef struct	s_zone
{
	struct s_zone	*next;
	struct s_zone	*prev;
}				t_zone;

typedef struct	s_allocs
{
	t_zone			*zones;
	t_block_free	*free_blocks;
}				t_allocs;

typedef struct	s_alloc_zone
{
	size_t	alloc_type;
	t_zone	*zone;
}				t_alloc_zone;

enum	e_allocs_sizes
{
	E_ALLOC_TINY = 0,
	E_ALLOC_SMALL,
	E_ALLOC_LARGE,
	E_ALLOC_NONE,
};

size_t			align_size(size_t size, size_t alignment);
unsigned		allocs_get_type(size_t request_size);
size_t			zone_map(t_zone **dst, size_t size, unsigned type);

t_block			*get_next_block(t_block const *block);
t_block			*block_init_zone(t_zone *zone, size_t zone_size, size_t size);
size_t			block_size(size_t size);

void			free(void *ptr);
void			*malloc(size_t size);
void			*realloc(void *ptr, size_t size);
void			*calloc(size_t numitems, size_t size);
void			show_alloc_mem();

#endif
