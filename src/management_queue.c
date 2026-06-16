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

#include "include/codexion.h"

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

void ft_remove_from_queue(t_queue *queue, t_coder *coder)
{
    t_nodo_coder *prev = NULL;
    t_nodo_coder *curr = queue->first;

    while (curr)
    {
        if (curr->coder == coder)
        {
            if (prev)
                prev->next = curr->next;
            else
                queue->first = curr->next;

            if (queue->last == curr)
                queue->last = prev;

            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void ft_remove_from_dongle_queue(t_coder *coder)
{
    t_dongle *left = coder->left_dongle;
    t_dongle *right = coder->right_dongle;

    pthread_mutex_lock(&left->queue_coders.lock);
    ft_remove_from_queue(&left->queue_coders, coder);
    pthread_mutex_unlock(&left->queue_coders.lock);

    pthread_mutex_lock(&right->queue_coders.lock);
    ft_remove_from_queue(&right->queue_coders, coder);
    pthread_mutex_unlock(&right->queue_coders.lock);
}