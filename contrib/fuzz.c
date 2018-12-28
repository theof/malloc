/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fuzz.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*       tvallee <tvallee@student.42.fr>          +#+#+#+#+#+   +#+           */
/*                                                     #+#    #+#             */
/*                                                    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This tiny program attempts to write again and again on memory regions
** claimed with malloc.
** If the malloc being tested relies on caller-reserved data to store metadata,
** it will probably lead to a crash, as it would be overwritten.
** Also, this programs tests for race conditions.
**
** Be aware that this program  DOES NOT check for caller-reserved data integrity
** between successive malloc/realloc/free calls
** 
**
** Possible improvements include:
** - ability to use a particular rand seed. This would help debugging in single
** thread mode.
** - writing patterns in memory instead of random. We could then periodically
** check for memory corruption
** - fix concurrent output
*/

#include "malloc.h"
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>

#define MEDIUM_SIZE 4096
#ifndef TINY_SIZE
# error TINY_SIZE should be defined as the `n' from 42's malloc subject
#endif

#ifndef MEDIUM_SIZE
# error MEDIUM_SIZE should be defined as the `m' from 42's malloc subject
#endif

#define NTHREAD_DEFAULT 1
#define DATA_TABLE_SIZE 200

struct				s_fuzz_data;

static uint32_t		get_random_size(unsigned *type);
static void			fuzz_putchar(char c);
static void			fuzz_putstr(char const *str);
static void			fuzz_putendl(char const *str);
static void			fuzz_putnbr(int n);
static void			fuzz_wrap_malloc(struct s_fuzz_data *current);
static void			fuzz_wrap_realloc(struct s_fuzz_data *current);
static void			fuzz_wrap_free(struct s_fuzz_data *current);
static void			fuzz_scribble(struct s_fuzz_data *current);

struct				s_fuzz_data
{
	void				*ptr;
	uint32_t			size;
	pthread_mutex_t		mutex;
};

struct s_fuzz_data	g_data_table[DATA_TABLE_SIZE] = {{.ptr = NULL, .size = 0}};

void				*fuzz_data_table(void *unused)
{
	struct s_fuzz_data	*current;

	(void)unused;
	while (true)
	{
		current = &(g_data_table[arc4random_uniform(DATA_TABLE_SIZE)]);
		if (pthread_mutex_lock(&current->mutex) != 0)
		{
			fuzz_putendl("pthread_mutex_lock error");
			abort();
		}

		if (current->ptr == NULL)
			fuzz_wrap_malloc(current);
		else {
			switch (arc4random_uniform(5))
			{
				case 0:
					fuzz_wrap_free(current);
					break;
				case 1:
					fuzz_wrap_realloc(current);
					break;
			}
		}
		fuzz_scribble(current);

		if (pthread_mutex_unlock(&current->mutex) != 0)
		{
			fuzz_putendl("pthread_mutex_unlock error");
			abort();
		}
	}

	return (NULL);
}

int					main(int ac, char **av)
{
	pthread_mutexattr_t	attr;
	int					opt;
	size_t				nthread;

	nthread = NTHREAD_DEFAULT;
	while ((opt = getopt(ac, av, "a:ql:cj:e")) != -1) {
		switch (opt) {
		case 'j':
			nthread = atoi(optarg);
			break;
		default:
			fuzz_putstr("Usage: ");
			fuzz_putstr(av[0]);
			fuzz_putendl(" [–j nthreads]\n"
					"\t-j set the number of threads (default 1)");
			exit(EXIT_FAILURE);
		}
	}

	pthread_t			thread_id[nthread];
	pthread_mutexattr_init(&attr);
	for (size_t i = 0; i < sizeof(g_data_table) / sizeof(g_data_table[0]); i++)
	{
		if (pthread_mutex_init(&g_data_table[i].mutex, &attr) != 0)
		{
			fuzz_putendl("pthread_mutex_init failed");
			abort();
		}
	}
	for(size_t i = 0; i < nthread; i++)
		pthread_create(&thread_id[i], NULL, fuzz_data_table, NULL);
	for(size_t i = 0; i < nthread; i++)
		pthread_join(thread_id[i], NULL);
	return (EXIT_SUCCESS);
};

/* ****************************** OPERATIONS ******************************** */

static void			fuzz_wrap_realloc(struct s_fuzz_data *current)
{
	unsigned	type;

	fuzz_putstr("realloc size: "); //TODO print all parameters
	fuzz_putnbr(current->size);
	fuzz_putstr(" => ");
	current->size = get_random_size(&type);
	fuzz_putnbr(current->size);
	fuzz_putchar(10);
	current->ptr = realloc(current->ptr, current->size);
	assert(!((uintmax_t)current->ptr % sizeof(uintmax_t)));
}

static void			fuzz_wrap_free(struct s_fuzz_data *current)
{
	fuzz_putstr("free"); //TODO print parameters
	free(current->ptr);
	current->ptr = NULL;
	fuzz_putendl(" done");
}

static void			fuzz_scribble(struct s_fuzz_data *current)
{
	if (current->ptr != NULL)
	{
		fuzz_putstr("scribbling available memory...");
		arc4random_buf(current->ptr, current->size);
		fuzz_putendl(" done");
	}
}

static void			fuzz_wrap_malloc(struct s_fuzz_data *current)
{
	static char const	type_map[3][6] = {"TINY", "SMALL", "LARGE"};
	unsigned			type;

	current->size = get_random_size(&type);

	fuzz_putstr("malloc(");
	fuzz_putnbr(current->size);
	fuzz_putstr(" /* ");
	fuzz_putstr(type_map[type]);
	fuzz_putstr(" */)...");

	current->ptr = malloc(current->size);
	assert(current->ptr != NULL);
	assert(!((uintmax_t)current->ptr % sizeof(uintmax_t)));

	fuzz_putendl(" done");
}

/* ****************************** UTILITIES ********************************* */

/*
** This helper balances allocation sizes evenly between the different zone types
** required in 42's malloc subject.
*/
static uint32_t		get_random_size(unsigned *type)
{
	uint32_t	ret;

	*type = arc4random_uniform(3);
	switch (*type)
	{
		case 0:
			ret = arc4random_uniform(TINY_SIZE + 1);
			break;
		case 1:
			do {
				ret = arc4random_uniform(MEDIUM_SIZE + 1);
			} while (ret <= TINY_SIZE);
			break;
		case 2:
			do {
				ret = arc4random_uniform(1024 * 1024 * 42);
			} while (ret <= MEDIUM_SIZE);
			break;
	}
	return (ret);
}

static void			fuzz_putchar(char c)
{
	write(STDOUT_FILENO, &c, 1);
}

static void			fuzz_putstr(char const *str)
{
	if (str)
		write(STDOUT_FILENO, str, strlen(str));
}

static void			fuzz_putendl(char const *str)
{
	fuzz_putstr(str);
	write(STDOUT_FILENO, "\n", 1);
}

static void			fuzz_putnbr(int n)
{
	unsigned int number;

	number = 0;
	if (n < 0)
	{
		fuzz_putchar('-');
		number = -n;
	}
	else
		number = n;
	if (number >= 10)
	{
		fuzz_putnbr(number / 10);
		fuzz_putnbr(number % 10);
	}
	if (number < 10)
		fuzz_putchar(number + '0');
}
