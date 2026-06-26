/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 10:02:28 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/26 10:09:04 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

void	ft_destroy_mutex(t_data *data)
{
	int	index;

	index = 0;
	while (data->number_coders != index)
	{
		pthread_mutex_destroy(&data->coder[index].lock_burnout);
		pthread_mutex_destroy(&data->coder[index].lock_finished);
		pthread_mutex_destroy(&data->dongle[index].lock);
		pthread_cond_destroy(&data->dongle[index].cond);
		index++;
	}
	pthread_mutex_destroy(&data->lock_print);
	pthread_mutex_destroy(&data->lock_program);
}

static void	ft_free_queue(t_queue *queue)
{
	t_nodo_coder	*current;
	t_nodo_coder	*next;

	current = queue->first;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	queue->first = NULL;
	queue->last = NULL;
}

void	ft_free_memory(t_data *data)
{
	int	index;

	index = 0;
	while (index < data->number_coders)
	{
		ft_free_queue(&data->dongle[index].queue_coders);
		index++;
	}
	free(data->coder);
	free(data->dongle);
}
