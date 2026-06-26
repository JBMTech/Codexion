/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_dfe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 10:09:42 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/26 10:05:15 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int	ft_has_higher_priority(long long dead1, long long dead2)
{
	return (dead1 < dead2);
}

void	ft_insert_front(t_queue *queue, t_nodo_coder *node)
{
	if (!queue->first)
	{
		queue->first = node;
		queue->last = node;
		return ;
	}
	node->next = queue->first;
	queue->first = node;
}

void	ft_insert_back(t_queue *queue, t_nodo_coder *node)
{
	if (!queue->first)
	{
		queue->first = node;
		queue->last = node;
		return ;
	}
	queue->last->next = node;
	queue->last = node;
}

void	ft_find_position(t_queue *q, t_nodo_coder *n,
				t_nodo_coder **prev, t_nodo_coder **curr)
{
	long long	deadline;

	deadline = ft_get_coder_deadline(n->coder);
	*prev = q->first;
	*curr = q->first->next;
	while (*curr)
	{
		if (ft_has_higher_priority(deadline,
				ft_get_coder_deadline((*curr)->coder)))
			return ;
		*prev = *curr;
		*curr = (*curr)->next;
	}
}

int	ft_add_to_queue_edf(t_coder *coder, t_queue *queue)
{
	t_nodo_coder	*node;
	t_nodo_coder	*prev;
	t_nodo_coder	*curr;
	long long		deadline;

	node = ft_create_node(coder);
	if (!node)
		return (1);
	deadline = ft_get_coder_deadline(coder);
	if (!queue->first || ft_has_higher_priority(deadline,
			ft_get_coder_deadline(queue->first->coder)))
	{
		ft_insert_front(queue, node);
		return (0);
	}
	ft_find_position(queue, node, &prev, &curr);
	if (!curr)
		ft_insert_back(queue, node);
	else
	{
		prev->next = node;
		node->next = curr;
	}
	return (0);
}
