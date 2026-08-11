/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:11:20 by marvin            #+#    #+#             */
/*   Updated: 2026/08/11 13:35:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_max_cooldown(long long left_cooldown, long long right_cooldown)
{
	if (left_cooldown > right_cooldown)
		return (left_cooldown);
	return (right_cooldown);
}

void	take_dongles(t_coder *coder, t_table *table)
{
	long long	priority;

	if (table->rules.is_fifo == 1)
		priority = 0;
	else
		priority = coder->last_compile_time + coder->rules->time_to_burnout;
	pthread_mutex_lock(&table->arbiter);
	push_heap(&table->waitlist, coder->id, priority);
	while (table->simulation_running == 1
		&& (table->dongle_state[coder->left_dongle_id] == 1
			|| table->dongle_state[coder->right_dongle_id] == 1
			|| table->waitlist.array[0].coder_id != coder->id)
	)
		pthread_cond_wait(&table->queue, &table->arbiter);
	remove_heap_node(&table->waitlist, coder->id);
	if (table->simulation_running == 1)
	{
		table->dongle_state[coder->left_dongle_id] = 1;
		table->dongle_state[coder->right_dongle_id] = 1;
	}
	pthread_mutex_unlock(&table->arbiter);
}

void	release_dongles(t_coder *coder, t_table *table)
{
	long long	ready_time;

	pthread_mutex_lock(&table->arbiter);
	table->dongle_state[coder->id - 1] = 0;
	table->dongle_state[coder->id % table->rules.num_coders] = 0;
	ready_time = get_time() + coder->rules->dongle_cooldown;
	table->dongle_cooldown_end[coder->id - 1] = ready_time;
	table->dongle_cooldown_end[coder->id
		% table->rules.num_coders] = ready_time;
	pthread_cond_broadcast(&table->queue);
	pthread_mutex_unlock(&table->arbiter);
}
