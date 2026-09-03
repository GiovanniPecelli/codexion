/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecelli <gpecelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 11:50:28 by marvin            #+#    #+#             */
/*   Updated: 2026/09/03 10:09:42 by gpecelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_usleep(long long time_to_compile, t_table *table)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < time_to_compile)
	{
		if (get_simulation_status(table) == 0)
			break ;
		usleep(500);
	}
}

int	get_simulation_status(t_table *table)
{
	int	status;

	pthread_mutex_lock(&table->state_mutex);
	status = table->simulation_running;
	pthread_mutex_unlock(&table->state_mutex);
	return (status);
}

void	set_simulation_status(t_table *table, int status)
{
	pthread_mutex_lock(&table->state_mutex);
	table->simulation_running = status ;
	pthread_mutex_unlock(&table->state_mutex);
}

void	print_status(t_table *table, int id, char *status)
{
	pthread_mutex_lock(&table->print_mutex);
	if (get_simulation_status(table) == 1)
		printf("%lld %d %s\n", get_time() - table->start_time, id, status);
	pthread_mutex_unlock(&table->print_mutex);
}

void	finish_program(t_table *table)
{
	pthread_mutex_destroy(&table->arbiter);
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->state_mutex);
	pthread_cond_destroy(&table->queue);
	free(table->dongle_state);
	free(table->coders);
	free(table->waitlist.array);
	free(table->dongle_cooldown_end);
}
