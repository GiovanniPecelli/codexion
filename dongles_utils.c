/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecelli <gpecelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:58:07 by gpecelli          #+#    #+#             */
/*   Updated: 2026/09/03 10:09:49 by gpecelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

long long	get_max_cooldown(long long left_cooldown, long long right_cooldown)
{
	if (left_cooldown > right_cooldown)
		return (left_cooldown);
	return (right_cooldown);
}

int	share_dongles(t_coder *coder1, t_coder *coder2)
{
	if (coder1->left_dongle_id == coder2->left_dongle_id
		|| coder1->left_dongle_id == coder2->right_dongle_id)
		return (1);
	if (coder1->right_dongle_id == coder2->right_dongle_id
		|| coder1->right_dongle_id == coder2->left_dongle_id)
		return (1);
	return (0);
}

/*
** IF (get_time() < max_cooldown):
	say: if the dongle is already free but is in the cooldown state
	set the clock
** ELSE:
	it mean that a thread is using the dongle and when it will end
	his process, it will call "broadcast" to wake-up the sleep thread
** get_max_cooldown() determines the time needed for both dongles to be ready.
** TS: is the time required from timedwait() to set the wake-up hour
*/
void	set_time(t_table *table, t_coder *coder)
{
	long long		max_cooldown;
	struct timespec	ts;

	max_cooldown = get_max_cooldown(
			table->dongle_cooldown_end[coder->left_dongle_id],
			table->dongle_cooldown_end[coder->right_dongle_id]);
	if (get_time() < max_cooldown)
	{
		ts.tv_sec = max_cooldown / 1000;
		ts.tv_nsec = (max_cooldown % 1000) * 1000000;
		pthread_cond_timedwait(&table->queue, &table->arbiter, &ts);
	}
	else
		pthread_cond_wait(&table->queue, &table->arbiter);
}
