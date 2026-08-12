/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 11:50:28 by marvin            #+#    #+#             */
/*   Updated: 2026/08/12 11:56:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Retrieves the current real time using gettimeofday.
** Converts seconds and microseconds into a single millisecond timestamp.
** Returns -1 on failure.
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
	if (table->simulation_running == 1)
		printf("%lld %d %s\n", get_time(), id, status);
	pthread_mutex_unlock(&table->print_mutex);
}

void	finish_program(t_table *table)
{
	pthread_mutex_destroy(&table->arbiter);
	pthread_mutex_destroy(&table->print_mutex);
	pthread_cond_destroy(&table->queue);

	free(table->dongle_state);
	free(table->coders);
	free(table->waitlist.array);
	free(table->dongle_cooldown_end);
}
