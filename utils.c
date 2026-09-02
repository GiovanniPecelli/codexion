/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 11:50:28 by marvin            #+#    #+#             */
/*   Updated: 2026/09/02 18:21:57 by marvin           ###   ########.fr       */
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

/*
** Retrieves the current real time using gettimeofday.
** Converts seconds and microseconds into a single millisecond timestamp.
** Returns -1 on failure.
** Real-time includes the time the CPU spent working on other background
** programs while your program was paused waiting its turn.
** CPU time only counts the time the CPU was actually executing your code.
**		• Waiting for I/O (Input/Output): If your program has to read a large
**		  file from the hard drive or wait for a network response, it pauses.
*/
long long	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
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
