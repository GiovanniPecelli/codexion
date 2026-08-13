/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 11:50:22 by marvin            #+#    #+#             */
/*   Updated: 2026/08/13 17:24:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Handles the primary logic for each coder thread, including
** grabbing dongles and simulating the coding process.
*/
void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->rules->num_coders == 1)
	{
		while (get_simulation_status(coder->table) == 1)
			usleep(1000);
		return (NULL);
	}
	while (get_simulation_status(coder->table) == 1)
	{
		take_dongles(coder, coder->table);
		print_status(coder->table, coder->id, "has taken a dongle");
		print_status(coder->table, coder->id, "has taken a dongle");
		print_status(coder->table, coder->id, "is compiling");
		coder->last_compile_time = get_time();
		usleep(coder->rules->time_to_compile * 1000);
		coder->compiles_done++;
		release_dongles(coder, coder->table);
		print_status(coder->table, coder->id, "is debugging");
		usleep(coder->rules->time_to_debug * 1000);
		print_status(coder->table, coder->id, "is refactoring");
		usleep(coder->rules->time_to_refactor * 1000);
	}
	return (NULL);
}

int	start_simulation(t_table *table)
{
	int			i;
	pthread_t	monitor_thread;

	i = -1;
	while (++i < table->rules.num_coders)
	{
		if (pthread_create(&table->coders[i].thread_id, NULL, &coder_routine,
				&table->coders[i]) != 0)
		{
			return (-1);
		}
	}
	pthread_create(&monitor_thread, NULL, &monitor_routine, table);
	i = 0;
	while (i < table->rules.num_coders)
		pthread_join(table->coders[i++].thread_id, NULL);
	pthread_join(monitor_thread, NULL);
	return (0);
}

/*
** Program entry point. Initializes rules and table structures.
** Exits with 1 on failure, or proceeds to simulation on success.
*/
int	main(int argc, char **argv)
{
	t_table		table;

	if (init_rules(argc, argv, &table.rules) == -1)
		return (-1);
	if (init_table(&table) == -1)
		return (-1);
	if (start_simulation(&table) == -1)
	{
		finish_program(&table);
		return (-1);
	}
	finish_program(&table);
	return (0);
}
