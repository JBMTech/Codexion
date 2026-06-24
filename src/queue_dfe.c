/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_dfe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 10:09:42 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/24 10:09:46 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

t_nodo_coder	*ft_create_node(t_coder *coder)
{
	t_nodo_coder	*node;

	node = malloc(sizeof(t_nodo_coder));
	if (!node)
		return (NULL);
	node->coder = coder;
	node->next = NULL;
	return (node);
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

int	ft_add_to_queue_edf(t_coder *coder, t_queue *queue)
{
	t_nodo_coder	*node;
	long long		deadline;

	node = ft_create_node(coder);
	if (!node)
		return (1);
	deadline = ft_get_coder_deadline(coder);
	if (!queue->first)
		return (ft_insert_front(queue, node), 0);
	if (deadline < ft_get_coder_deadline(queue->first->coder))
		return (ft_insert_front(queue, node), 0);
	while (queue->first->next)
	{
		if (deadline < ft_get_coder_deadline(queue->first->next->coder))
		{
			queue->first->next = node;
			node->next = queue->first->next;
			return (0);
		}
		queue->first = queue->first->next;
		queue->first->next = queue->first->next->next;
	}
	queue->last->next = node;
	queue->last = node;
	return (0);
}
