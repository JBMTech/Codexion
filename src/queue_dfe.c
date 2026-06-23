
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

void	ft_find_prev_curr_edf(t_queue *queue,
	t_nodo_coder **prev, t_nodo_coder **curr,
	long long deadline)
{
	*prev = queue->first;
	*curr = queue->first->next;
	while (*curr)
	{
		if (deadline < ft_get_coder_deadline((*curr)->coder))
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
	if (!queue->first)
		return (ft_insert_front(queue, node), 0);
	if (deadline < ft_get_coder_deadline(queue->first->coder))
		return (ft_insert_front(queue, node), 0);
	ft_find_prev_curr_edf(queue, &prev, &curr, deadline);
	if (!curr)
		ft_insert_back(queue, node);
	else
	{
		prev->next = node;
		node->next = curr;
	}
	return (0);
}
