/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brutalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*       gpouyat <gpouyat@student.42.fr>          +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*                                                    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*	./brut -e => error
*	./brut -c => calloc
*	./brut -p => pthread
*	./brut -l => limit
*/

#include "../include/malloc.h"
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>

// Wrapper for define
#define TINY_SIZE_BRUT TINY_SIZE
#define MEDIUM_SIZE_BRUT SMALL_SIZE

typedef struct	s_flag
{
	bool	quiet;
	bool	calloc;
	size_t	limit;
	bool	checkError;
	int		align;
	int		nthread;
}				t_flag;

typedef struct	s_data
{
	void				*ptr;
	uint32_t			size;
	pthread_mutex_t	mutex;
}				t_data;

t_flag		g_flag = {
	.quiet = false,
	.calloc = false,
	.limit = 0,
	.nthread = 1,
	.checkError = false,
	.align = 0
};

static void		ft_putchar(char c)
{
	write(STDOUT_FILENO, &c, 1);
}

static void		ft_putstr(char const *str)
{
	if (str)
		write(STDOUT_FILENO, str, strlen(str));
}

static void		ft_putendl(char const *str)
{
	ft_putstr(str);
	write(STDOUT_FILENO, "\n", 1);
}

static void		ft_putnbr(int n)
{
	unsigned int number;

	number = 0;
	if (n < 0)
	{
		ft_putchar('-');
		number = -n;
	}
	else
		number = n;
	if (number >= 10)
	{
		ft_putnbr(number / 10);
		ft_putnbr(number % 10);
	}
	if (number < 10)
		ft_putchar(number + '0');
}


char		g_type_map[3][6] = {"TINY", "SMALL", "LARGE"};


static uint32_t	get_random_size(unsigned *type)
{
	uint32_t	ret;

	*type = arc4random_uniform(3);
	switch (*type)
	{
		case 0:
			ret = arc4random_uniform(TINY_SIZE_BRUT + 1);
			break;
		case 1:
			do {
				ret = arc4random_uniform(MEDIUM_SIZE_BRUT + 1);
			} while (ret <= TINY_SIZE_BRUT);
			break;
		case 2:
			do {
				ret = arc4random_uniform(1024 * 1024 * 42);
			} while (ret <= MEDIUM_SIZE_BRUT);
			break;
	}
	return (ret);
}

static void	fill_mem(t_data *current)
{
	if (current->ptr != NULL)
	{
		g_flag.quiet ? : ft_putstr("=> filling available memory: ");
		arc4random_buf(current->ptr, current->size);
		g_flag.quiet ? : ft_putendl("OK !");
	}
}

static void use_malloc(t_data *current, unsigned *type)
{
	current->size = get_random_size(type);

	g_flag.quiet ? : ft_putstr(" malloc ");
	g_flag.quiet ? : ft_putstr(g_type_map[*type]);
	g_flag.quiet ? : ft_putstr(" size ");
	g_flag.quiet ? : ft_putnbr(current->size);
	g_flag.quiet ? : ft_putchar(10);

	current->ptr = malloc(current->size);
	g_flag.align ? assert(!((long)current->ptr % g_flag.align)): NULL;
	
	if (current->ptr == NULL)
	{
		ft_putendl("malloc returned NULL !");
		exit(1);
	}
}

static void use_calloc(t_data *current, unsigned *type)
{
	size_t	second_arg;
	current->size = get_random_size(type);

	if (arc4random_uniform(2) == 1)
		second_arg = arc4random_uniform(10);
	else
		second_arg = 0;
	g_flag.quiet ? : ft_putstr(" calloc ");
	g_flag.quiet ? : ft_putstr(g_type_map[*type]);
	g_flag.quiet ? : ft_putstr(" size ");
	g_flag.quiet ? : ft_putnbr(current->size);
	g_flag.quiet ? : ft_putstr(", ");
	g_flag.quiet ? : ft_putnbr(second_arg);
	g_flag.quiet ? : ft_putchar(10);

	current->ptr = calloc(current->size, second_arg);
	g_flag.align ? assert(!((long)current->ptr % g_flag.align)): NULL;
	current->size = current->size * second_arg;
	if (!current->size)
		current->size = 1;
	if (current->ptr == NULL)
	{
		ft_putendl("calloc returned NULL !");
		exit(1);
	}
}

t_data		g_table[200] = { {.ptr = NULL, .size = 0} };

void *brut(void *argv)
{
	t_data		*current;
	unsigned	type;
	size_t		count;

	(void)argv;
	count = 0;
	g_flag.quiet ? : ft_putendl("=========BEGIN BRUTALIZE=========");
	while (!g_flag.limit || count++ < g_flag.limit)
	{
		g_flag.quiet ? : ft_putstr("\n=> new iteration: ");

		current = &(g_table[arc4random_uniform(200)]);
		if (pthread_mutex_lock(&current->mutex) != 0)
		{
			ft_putendl("pthread_mutex_lock error");
			abort();
		}
		if (current->ptr == NULL)
			g_flag.calloc ? use_calloc(current, &type) : use_malloc(current, &type);
		else {
			switch (arc4random_uniform(5))
			{
				case 0:
					g_flag.quiet ? : ft_putendl("free");
					free(current->ptr);
					current->ptr = NULL;
					break;
				case 1:
					g_flag.quiet ? : ft_putstr("realloc size: ");
					g_flag.quiet ? : ft_putnbr(current->size);
					g_flag.quiet ? : ft_putstr(" => ");
					current->size = get_random_size(&type);
					g_flag.quiet ? : ft_putnbr(current->size);
					g_flag.quiet ? : ft_putchar(10);
					current->ptr = realloc(current->ptr, current->size);
					g_flag.align ? assert(!((long)current->ptr % g_flag.align)): NULL;
					break;
				default:
					g_flag.quiet ? : ft_putendl("no op");
					break;
			}
		}
		fill_mem(current);
		if (pthread_mutex_unlock(&current->mutex) != 0)
		{
			ft_putendl("pthread_mutex_unlock error");
			abort();
		}
	}
	return NULL;
}


void		spawn_threads(void)
{
	pthread_t			thread_id[g_flag.nthread];
	pthread_mutexattr_t	attr;
	size_t		count;

	count = 0;
	pthread_mutexattr_init(&attr);
	for (int i = 0; i < sizeof(g_table) / sizeof(g_table[0]); i++)
	{
		if (pthread_mutex_init(&g_table[i].mutex, &attr) != 0)
		{
			ft_putendl("pthread_mutex_init failed");
			abort();
		}
	}
	while (!g_flag.limit || count++ < g_flag.limit)
	{
		for(int i = 0; i < g_flag.nthread; i++)
			pthread_create(&thread_id[i], NULL, brut, NULL);
		for(int i = 0; i < g_flag.nthread; i++)
		{
				pthread_join(thread_id[i], NULL);
				ft_putnbr(i);
				ft_putendl(" + pthread OK");
		}
		count++;
	}
}

static void check_error()
{
	char *str;

	ft_putendl(" ================ BEGIN CHECK ERROR ================ ");
	str = malloc(42);
	free(str + 1);
	free(str - 1);
	if (realloc(str + 1, 43) || realloc(str - 1, 43))
		ft_putendl("realloc error fail test");
	free(str);
	if (realloc(str, 42))
		ft_putendl("realloc error fail test");
	ft_putendl("================ END CHECK ERROR ================");
}


int main(int ac, char **av)
{
	int			opt;

	while ((opt = getopt(ac, av, "a:ql:cj:e")) != -1) {
		switch (opt) {
		case 'q':
			g_flag.quiet = true;
			break;
		case 'l':
			g_flag.limit = atoi(optarg);
			break;
		case 'c':
			g_flag.calloc = true;
			break;
		case 'j':
			g_flag.nthread = atoi(optarg);
			break;
		case 'a':
			g_flag.align = atoi(optarg);
			break;
		case 'e':
			check_error();
			return (0);
			break;
		default: /* '?' */
			ft_putendl("Usage: ./brut [-qce] [–alj number] \n"
					"\t-q: quiet (suppress all output but pthread logs)\n"
					"\t-l: limit set limit number of loop\n"
					"\t-c: use calloc \n"
					"\t-j set the number of threads (default 1)\n"
					"\t-e launch test error\n"
					"\t-a check align\n");
			exit(EXIT_FAILURE);
		}
	}

	spawn_threads();
	exit(0);
};
