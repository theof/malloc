/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/05 16:42:41 by tvallee           #+#    #+#             */
/*   Updated: 2017/12/18 14:19:26 by tvallee          ###   ########.fr       */
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
# define TINY_BLOCK_SIZE FLOOR_8((TINY_ZONE_SIZE - sizeof(t_zone)) / 102)
# define TINY_ZONE_SIZE (2 * (size_t)getpagesize())

# define SMALL_SIZE (SMALL_BLOCK_SIZE - 2 * sizeof(t_block))
# define SMALL_BLOCK_SIZE FLOOR_8((SMALL_ZONE_SIZE - sizeof(t_zone)) / 126)
# define SMALL_ZONE_SIZE (32 * (size_t)getpagesize())

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
unsigned		allocs_get_type_block(size_t block_size);
int				allocs_assert_available_block_type(size_t size, unsigned type);
unsigned		allocs_is_ours(void *ptr);

size_t			zone_map(t_zone **dst, size_t size, unsigned type);
void			zone_unmap(t_zone *zone, unsigned type);

t_block_free	*block_push_free_list(t_block *block, unsigned type);
t_block_free	*block_replace_free_list(t_block_free *old, t_block *new, unsigned type);
t_block			*block_pop_free_list(t_block_free *block, unsigned type);
t_block			*get_next_block(t_block const *block);
t_block			*get_prev_block(t_block const *block);

t_block_free	*coalesce(t_block *current, unsigned type);

t_block			*block_shrink(t_block *block, size_t size, unsigned type);
t_block			*block_enlarge(t_block *block, size_t diff, unsigned type);

size_t			block_size(size_t size);
t_block_free	*block_init_zone(t_zone *zone, size_t zone_size, unsigned type);
t_block			*block_create(t_block_free *available, size_t size, unsigned type);
void			block_update_footer(t_block *block);
t_block			*block_fit(size_t size, unsigned type);

void			free(void *ptr);
void			*malloc(size_t size);
void			*realloc(void *ptr, size_t size);
void			show_alloc_mem();

#endif
