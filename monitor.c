/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 17:34:20 by marvin            #+#    #+#             */
/*   Updated: 2026/08/08 17:34:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules.num_coders)
	{
		if ((get_time() - table->coders[i].last_compile_time)
			>= table->rules.time_to_burnout)
		{
			pthread_mutex_lock(&table->print_mutex);
			printf("%lld %d burned out\n", get_time(), table->coders[i].id);
			table->simulation_running = 0;
			pthread_mutex_unlock(&table->print_mutex);
			return (0);
		}
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
		if (table->coders[i].compiles_done >= table->rules.compiles_required)
			finished++;
		i++;
	}
	if (finished == table->rules.num_coders)
	{
		table->simulation_running = 0;
		return (0);
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (table->simulation_running == 1)
	{
		if (check_burnout(table) == 0 || check_complete(table) == 0)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
