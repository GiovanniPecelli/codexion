/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecelli <gpecelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 17:34:20 by marvin            #+#    #+#             */
/*   Updated: 2026/09/03 10:41:23 by gpecelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	terminate_simulation(t_table *table)
{
	set_simulation_status(table, 0);
	pthread_mutex_lock(&table->arbiter);
	pthread_cond_broadcast(&table->queue);
	pthread_mutex_unlock(&table->arbiter);
}

static int	is_burnout(t_table *table, long long last_time, int i)
{
	if ((get_time() - last_time) >= table->rules.time_to_burnout)
	{
		pthread_mutex_lock(&table->print_mutex);
		printf("%lld %d burned out\n",
			get_time() - table->start_time, table->coders[i].id);
		pthread_mutex_unlock(&table->print_mutex);
		terminate_simulation(table);
		return (0);
	}
	return (1);
}

static int	check_burnout(t_table *table)
{
	long long	last_time;
	int			done;
	int			i;

	i = 0;
	while (i < table->rules.num_coders)
	{
		pthread_mutex_lock(&table->state_mutex);
		last_time = table->coders[i].last_compile_time;
		done = table->coders[i].compiles_done;
		pthread_mutex_unlock(&table->state_mutex);
		if (table->rules.compiles_required > 0
			&& done >= table->rules.compiles_required)
		{
			i++;
			continue ;
		}
		is_burnout(table, last_time, i);
		i++;
	}
	return (1);
}

static int	check_complete(t_table *table)
{
	int	i;
	int	finished;

	if (table->rules.compiles_required == 0)
		return (1);
	i = 0;
	finished = 0;
	while (i < table->rules.num_coders)
	{
		pthread_mutex_lock(&table->state_mutex);
		if (table->coders[i].compiles_done >= table->rules.compiles_required)
			finished++;
		pthread_mutex_unlock(&table->state_mutex);
		i++;
	}
	if (finished == table->rules.num_coders)
	{
		terminate_simulation(table);
		return (0);
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (get_simulation_status(table) == 1)
	{
		if (check_burnout(table) == 0 || check_complete(table) == 0)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
