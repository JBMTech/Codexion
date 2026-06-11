/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   management_queue.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:58:06 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:58:08 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Añadir a queue
int ft_add_to_queue(t_coder *coder_to_add, t_queue *queue)
{
    t_nodo_coder *node;

    node = malloc(sizeof(t_nodo_coder));
    if (node == NULL)
        return (1);
    node->coder = coder_to_add;
    node->next = NULL;
    if (queue->first == NULL)
    {
        queue->first = node;
        queue->last = node;
    }
    else
    {
        queue->last->next = node;
        queue->last = node;
    }
    return (0);
}

// Remover de queue
int ft_remove_from_queue(t_queue *queue)
{
    t_nodo_coder *tmp;

    if (queue->first == NULL)
        return (1);
    tmp = queue->first;
    queue->first = tmp->next;
    if (queue->first == NULL)
        queue->last = NULL;
    free (tmp);
    return (0);
}

// Manejamos el Queue de Coders
void ft_fifo_manager_queue(t_data *data, t_coder *coder)
{
    ft_add_to_queue(coder, &data->queue_coders);
    while ((ft_get_active_program(data) == 1) && 
    (data->queue_coders.first != coder) || ft_check_take_dongle(coder) == 1)
    {
        if ((ft_get_active_program(data) == 1) && data->queue_coders.first == coder)
        {
            pthread_mutex_unlock(&data->queue_coders.lock);
            usleep(800);
            pthread_mutex_lock(&data->queue_coders.lock);
        }
        else
            pthread_cond_wait(&data->queue_coders.cond, &data->queue_coders.lock);
    }
}

// Generamos FIFO
int ft_scheduler_fifo(t_data *data, t_coder *coder, char *action)
{
    pthread_mutex_lock(&data->queue_coders.lock);
    if (strcmp(action, ADD) == 0)
        ft_fifo_manager_queue(data, coder);
    else if (strcmp(action, REMOVE) == 0)
    {
        ft_remove_from_queue(&data->queue_coders);
        pthread_cond_broadcast(&data->queue_coders.cond);
    }
    pthread_mutex_unlock(&data->queue_coders.lock);
    return (0);
}