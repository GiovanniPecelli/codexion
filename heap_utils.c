/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 16:52:53 by marvin            #+#    #+#             */
/*   Updated: 2026/08/11 11:43:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_heap(t_table *table)
{
	table->waitlist.array = malloc(sizeof(t_heap_node)
			* table->rules.num_coders);
	if (!table->waitlist.array)
		return (-1);
	table->waitlist.size = 0;
	table->waitlist.ticket_gen = 0;
	table->waitlist.max_size = table->rules.num_coders;
	return (1);
}

void	push_heap(t_heap *heap, int coder_id, long long priority)
{
	int	index;

	index = heap->size;
	heap->array[index].coder_id = coder_id;
	heap->array[index].ticket = heap->ticket_gen;
	heap->ticket_gen = heap->ticket_gen + 1;
	heap->array[index].priority = priority;
	heapify_up(heap, index);
	heap->size = heap->size + 1;
}
