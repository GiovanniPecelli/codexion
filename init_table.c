/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 14:43:26 by marvin            #+#    #+#             */
/*   Updated: 2026/09/02 13:05:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Allocates memory on the heap for the coders and dongle_state arrays.
** Returns -1 if malloc fails, preventing memory leaks.
*/
int	table_allocation(t_table *table)
{
	table->dongle_state = malloc(sizeof(int) * table->rules.num_coders);
	if (table->dongle_state == NULL)
		return (-1);
	table->coders = malloc(sizeof(t_coder) * table->rules.num_coders);
	if (table->coders == NULL)
	{
		free(table->dongle_state);
		return (-1);
	}
	table->dongle_cooldown_end = malloc(sizeof(long long)
			* table->rules.num_coders);
	if (table->dongle_cooldown_end == NULL)
	{
		free(table->dongle_state);
		free(table->coders);
		return (-1);
	}
	return (0);
}

/*
** right_dongle_id is (i + 1) % table->rules.num_coders;
** IF ex: 5 > num_coders(4) -> 5 % 4 = 1
** last coder's right dongle is the first coder's left dongle
*/
static void	init_coders(t_table *table)
{
	int	i;

	table->simulation_running = 1;
	i = 0;
	while (i < table->rules.num_coders)
	{
		table->dongle_state[i] = 0;
		table->dongle_cooldown_end[i] = 0;
		table->coders[i].id = i + 1;
		table->coders[i].compiles_done = 0;
		table->coders[i].last_compile_time = 0;
		table->coders[i].left_dongle_id = i;
		table->coders[i].right_dongle_id = (i + 1) % table->rules.num_coders;
		table->coders[i].rules = &table->rules;
		table->coders[i].table = table;
		i++;
	}
}

/*
** Initializes mutexes and prepares the table.
** Uses the modulo operator (%) to create a "round" table architecture,
** assigning the correct left and right dongles to each coder.
*/
int	init_table(t_table *table)
{
	if (table_allocation(table) == -1)
		return (-1);
	if (init_heap(table) == -1)
		return (-1);
	if (pthread_mutex_init(&table->arbiter, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&table->state_mutex, NULL) != 0)
		return (-1);
	if (pthread_cond_init(&table->queue, NULL) != 0)
		return (-1);
	init_coders(table);
	return (0);
}
