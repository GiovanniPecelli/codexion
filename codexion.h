/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 13:00:46 by marvin            #+#    #+#             */
/*   Updated: 2026/08/11 11:49:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int				num_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				compiles_required;
	int				dongle_cooldown;
	int				is_fifo;
}					t_rules;

/*
** struct s_table	*table;
** is a previous declaration bc t_table struct come after
** if we write only "s_table *table" - C give an error
** -- *rules is a pointer to the bag rules
*/
typedef struct s_coder
{
	int				id;
	int				compiles_done;
	int				left_dongle_id;
	int				right_dongle_id;
	long long		last_compile_time;
	pthread_t		thread_id;
	t_rules			*rules;
	struct s_table	*table;
}					t_coder;

typedef struct s_heap_node
{
	int				coder_id;
	int				ticket;
	long long		priority;
}					t_heap_node;

typedef struct s_heap
{
	t_heap_node		*array;
	int				size;
	int				max_size;
	int				ticket_gen;
}					t_heap;

/*
** pthread_cond_t queue - is a kernel managed waiting list
** broadcast: wake up all threads
** signal: wake up only one thread
** -- rules is a bag (type t_rules) inside the table memory
*/
typedef struct s_table
{
	t_rules			rules;
	t_coder			*coders;
	int				*dongle_state;
	long long		*dongle_cooldown_end;
	int				simulation_running;
	pthread_mutex_t	arbiter;
	pthread_mutex_t	print_mutex;
	pthread_cond_t	queue;
	t_heap			waitlist;
}					t_table;

/*
** init_rules.c
*/
int					fill_rules(char **argv, t_rules *rules);
int					is_number(char *arg);
int					init_rules(int argc, char **argv, t_rules *rules);

/*
** init_table.c
*/
int					table_allocation(t_table *table);
int					init_table(t_table *table);

/*
** dongles_utils.c / utils.c
*/
void				take_dongles(t_coder *coder, t_table *table);
void				release_dongles(t_coder *coder, t_table *table);
long long			get_time(void);
void				print_status(t_table *table, int id, char *status);

/*
** codexion.c / monitor.c
*/
void				*coder_routine(void *arg);
void				*monitor_routine(void *arg);
/*
** heap.c / heap_utils.c
*/
int					init_heap(t_table *table);
void				push_heap(t_heap *heap, int coder_id, long long priority);
void				pop_heap(t_heap *heap);
void				remove_heap_node(t_heap *heap, int coder_id);
void				heapify_up(t_heap *heap, int index);
void				heapify_down(t_heap *heap, int index);

#endif