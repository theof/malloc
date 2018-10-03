/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brutalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpouyat <gpouyat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 18:05:32 by tvallee           #+#    #+#             */
/*   Updated: 2018/10/03 16:23:28 by gpouyat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/malloc.h"
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

// Wrapper for define
#define TINY_SIZE_BRUT TINY_SIZE
#define MEDIUM_SIZE_BRUT SMALL_SIZE


/*
*	./brut -e => error
*	./brut -c => calloc
*	./brut -p => pthread
*	./brut -l => limit
*/


void			ft_putchar(char c)
{
	write(STDOUT_FILENO, &c, 1);
}

void			ft_putstr(char const *str)
{
	if (str)
		write(1, str, strlen(str));
}

void		ft_putendl(char const *str)
{
	puts(str);
}

void	ft_putnbr(int n)
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

typedef struct	s_flag
{
	bool	quiet;
	bool	calloc;
	size_t	limit;
	bool	pthread;
	int		nbPthread;
	bool	checkError;
	int		align;
}				t_flag;

typedef struct	s_data
{
	void		*ptr;
	uint32_t	size;
}				t_data;

t_flag		g_flag = {
	false,
	false,
	0,
	false,
	2,
	false,
	0
};

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

void *brut(void *argv)
{
	t_data		table[200] = { {.ptr = NULL, .size = 0 } };
	t_data		*current;
	unsigned	type;
	size_t		count;

	(void)argv;
	count = 0;
	g_flag.quiet ? : ft_putendl("=========BEGIN BRUTALIZE=========");
	while (!g_flag.limit || count++ < g_flag.limit)
	{
		g_flag.quiet ? : ft_putstr("\n=> new iteration: ");

		current = &(table[arc4random_uniform(200)]);
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
	}
	return NULL;
}


void		check_pthread()
{
	pthread_t	thread_id[g_flag.nbPthread];
	size_t		count;

	count = 0;
	while (!g_flag.limit || count++ < g_flag.limit)
	{
		for(int i = 0; i < g_flag.nbPthread; i++)
			pthread_create(&thread_id[i], NULL, brut, NULL);
		for(int i = 0; i < g_flag.nbPthread; i++)
		{
				pthread_join(thread_id[i], NULL);
				ft_putnbr(i);
				puts(" + pthread OK");
		}
		count++;
	}
}

static void check_error()
{
	char *str;

	puts(" ================ BEGIN CHECK ERROR ================ ");
	str = malloc(42);
	free(str + 1);
	free(str - 1);
	if (realloc(str + 1, 43) || realloc(str - 1, 43))
		puts("realloc error fail test");
	free(str);
	if (realloc(str, 42))
		puts("realloc error fail test");
	puts("================ END CHECK ERROR ================");	
}


int main(int ac, char **av)
{
	int			opt;

	while ((opt = getopt(ac, av, "a:ql:cpn:e")) != -1) {
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
		case 'p':
			g_flag.pthread = true;
			break;
		case 'n':
			g_flag.nbPthread = atoi(optarg);
			break;
		case 'a':
			g_flag.align = atoi(optarg);
			break;
		case 'e':
			check_error();
			return (0);
			break;
		default: /* '?' */
			puts("Usage: ./brut [-qcpe] [–aln number] \n\t-q: quiet (no print except pthread log)\n\t-l: limit set limit number of loop\n\t-c: use calloc \n\t-p: pthread\n\t-n set the number of pthread(default 2)\n\t-e launch test error\n\t-a check align\n");
			exit(EXIT_FAILURE);
		}
	}

	if (g_flag.pthread)
		check_pthread();
	else
		brut(NULL);
	exit(0);
};
