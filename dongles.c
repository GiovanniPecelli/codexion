/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecelli <gpecelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:11:20 by marvin            #+#    #+#             */
/*   Updated: 2026/09/03 10:06:36 by gpecelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** take_dongles: (require: wait_for_dongles())
*/
static void	wait_for_dongles(t_coder *coder, t_table *table)
{
	while (get_simulation_status(coder->table) == 1
		&& (table->dongle_state[coder->left_dongle_id] == 1
			|| table->dongle_state[coder->right_dongle_id] == 1
			|| get_time() < get_max_cooldown(
				table->dongle_cooldown_end[coder->left_dongle_id],
				table->dongle_cooldown_end[coder->right_dongle_id])
			|| (table->waitlist.size > 0
				&& table->waitlist.array[0].coder_id != coder->id
				&& share_dongles(
					coder,
					&table->coders[table->waitlist.array[0].coder_id - 1]
				)))
	)
	{
		set_time(table, coder);
	}
}

/*
** take_dongles: (require: wait_for_dongles())
** Attempts to acquire both the left and right dongles for a coder.
** If the required dongles are either currently in use or still in their
** cooldown period, the thread will wait.
** 
** - pthread_cond_timedwait: Used when the thread needs to wait for a 
**   cooldown period to elapse. It guarantees the thread wakes up exactly
**   when the cooldown is over, even if no other thread signals it.
** - pthread_cond_wait: Used when the cooldown has already passed, but
**   the dongles are currently held by someone else, or the coder is not
**   the first in the priority queue. It waits indefinitely until another
**   coder releases their dongles and broadcasts a wakeup signal.
*/
void	take_dongles(t_coder *coder, t_table *table)
{
	long long	priority;

	if (table->rules.is_fifo == 1)
		priority = 0;
	else
		priority = coder->last_compile_time + coder->rules->time_to_burnout;
	pthread_mutex_lock(&table->arbiter);
	push_heap(&table->waitlist, coder->id, priority);
	wait_for_dongles(coder, table);
	remove_heap_node(&table->waitlist, coder->id);
	if (table->simulation_running == 1)
	{
		table->dongle_state[coder->left_dongle_id] = 1;
		table->dongle_state[coder->right_dongle_id] = 1;
	}
	pthread_mutex_unlock(&table->arbiter);
}

/*
** ready_time say whan a dongle resume from the cooldown time
*/
void	release_dongles(t_coder *coder, t_table *table)
{
	long long	ready_time;

	pthread_mutex_lock(&table->arbiter);
	table->dongle_state[coder->left_dongle_id] = 0;
	table->dongle_state[coder->right_dongle_id] = 0;
	ready_time = get_time() + coder->rules->dongle_cooldown;
	table->dongle_cooldown_end[coder->left_dongle_id] = ready_time;
	table->dongle_cooldown_end[coder->right_dongle_id] = ready_time;
	pthread_cond_broadcast(&table->queue);
	pthread_mutex_unlock(&table->arbiter);
}
