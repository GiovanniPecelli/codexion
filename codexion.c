/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecelli <gpecelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 11:50:22 by marvin            #+#    #+#             */
/*   Updated: 2026/09/03 10:37:06 by gpecelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coder_loop(t_coder *coder)
{
	while (get_simulation_status(coder->table) == 1)
	{
		if (coder->rules->compiles_required > 0
			&& coder->compiles_done >= coder->rules->compiles_required)
			break ;
		take_dongles(coder, coder->table);
		print_status(coder->table, coder->id, "has taken a dongle");
		print_status(coder->table, coder->id, "has taken a dongle");
		print_status(coder->table, coder->id, "is compiling");
		pthread_mutex_lock(&coder->table->state_mutex);
		coder->last_compile_time = get_time();
		pthread_mutex_unlock(&coder->table->state_mutex);
		ft_usleep(coder->rules->time_to_compile, coder->table);
		pthread_mutex_lock(&coder->table->state_mutex);
		coder->compiles_done++;
		pthread_mutex_unlock(&coder->table->state_mutex);
		release_dongles(coder, coder->table);
		print_status(coder->table, coder->id, "is debugging");
		ft_usleep(coder->rules->time_to_debug, coder->table);
		print_status(coder->table, coder->id, "is refactoring");
		ft_usleep(coder->rules->time_to_refactor, coder->table);
	}
}

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
	coder_loop(coder);
	return (NULL);
}

/*
** info:
1)  table->coders[i].last_compile_time is signed here because
    the program need to take the relative start_time of the program
2)  pthread_create(
        1.where pthread_create() will store the ID of the newly thread,
        2.use attributes to configure things (NULL = default attributes),
        3.the function that the new thread will execute.
        4.the argument that will be passed to the executable function
    )
3)  pthread_join()
    wait the finish for the each previously created thread
** 
*/
int	start_simulation(t_table *table)
{
	int			i;
	pthread_t	monitor_thread;

	table->start_time = get_time();
	i = -1;
	while (++i < table->rules.num_coders)
	{
		table->coders[i].last_compile_time = table->start_time;
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
		return (1);
	if (init_table(&table) == -1)
		return (1);
	if (start_simulation(&table) == -1)
	{
		finish_program(&table);
		return (1);
	}
	finish_program(&table);
	return (0);
}
