/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecelli <gpecelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 15:16:19 by marvin            #+#    #+#             */
/*   Updated: 2026/09/03 12:01:22 by gpecelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Array sorting rule:
** - Case A (FIFO): Priorities are all forced to 0.
**   Since they are equal (and equal to 0), the Heap uses 'ticket'.
**   The swap respects who arrived first (OS scheduler choice).
** - Case B (EDF): At the first round (and in ties), priorities are
**   equal but different from 0 (e.g. 200). In this case, the Heap
**   uses 'coder_id'. The swap ignores the random arrival order and
**   aligns them in a 100% deterministic way, as required!
*/
int	more_urgent(t_heap_node a, t_heap_node b)
{
	if (a.priority < b.priority)
		return (1);
	if (a.priority == b.priority)
	{
		if (a.priority == 0)
		{
			if (a.ticket < b.ticket)
				return (1);
		}
		else
		{
			if (a.coder_id < b.coder_id)
				return (1);
		}
	}
	return (0);
}

/*
** heapify_up move up the coder based on is urgent
*/
void	heapify_up(t_heap *heap, int index)
{
	int			parent;
	t_heap_node	temp;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (more_urgent(heap->array[index], heap->array[parent]) == 1)
		{
			temp = heap->array[index];
			heap->array[index] = heap->array[parent];
			heap->array[parent] = temp;
			index = parent;
		}
		else
			break ;
	}
}

/*
** heapify_up move down the coder based on is urgent
*/
void	heapify_down(t_heap *heap, int index)
{
	int			left;
	int			right;
	int			urgent_child;
	t_heap_node	temp;

	while ((2 * index + 1) < heap->size)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		urgent_child = left;
		if (right < heap->size && more_urgent(heap->array[right],
				heap->array[left]))
			urgent_child = right;
		if (more_urgent(heap->array[urgent_child], heap->array[index]))
		{
			temp = heap->array[index];
			heap->array[index] = heap->array[urgent_child];
			heap->array[urgent_child] = temp;
			index = urgent_child;
		}
		else
			break ;
	}
}

/*
** remove_heap_node: remove the node matching coder_id from the heap.
** The last node is moved into the freed slot, the heap size is reduced,
** and then the heap is repaired by bubbling the node up or down so the
** priority ordering remains valid.
** If the coder is not present, the function does nothing.
*/
void	remove_heap_node(t_heap *heap, int coder_id)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->array[i].coder_id == coder_id)
			break ;
		i++;
	}
	if (i == heap->size)
		return ;
	heap->array[i] = heap->array[heap->size - 1];
	heap->size = heap->size - 1;
	if (i < heap->size)
	{
		heapify_up(heap, i);
		heapify_down(heap, i);
	}
}
