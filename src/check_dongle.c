/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dongle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:53:13 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:53:16 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

void ft_request_dongles(t_coder *coder)
{
    ft_add_to_queue(coder, &coder->left_dongle->queue_coders);
    ft_add_to_queue(coder, &coder->right_dongle->queue_coders);
}

int ft_dongles_available(t_coder *coder)
{
    long long now;
    
    now = ft_get_now_time(coder->data);
    if (now < coder->left_dongle->cooldown)
        return (0);
    if (now < coder->right_dongle->cooldown)
        return (0);
    return (1);
}

int ft_is_first_both_queue(t_coder *coder)
{
    t_dongle *left;
    t_dongle *right;
    int result;

    left = coder->left_dongle;
    right = coder->right_dongle;
    result = 0;
    pthread_mutex_lock(&left->queue_coders.lock);
    pthread_mutex_lock(&right->queue_coders.lock);

    if (left->queue_coders.first &&
        right->queue_coders.first &&
        left->queue_coders.first->coder == coder &&
        right->queue_coders.first->coder == coder)
        result = 1;

    pthread_mutex_unlock(&right->queue_coders.lock);
    pthread_mutex_unlock(&left->queue_coders.lock);

    return result;
}

void ft_release_dongles(t_coder *coder, t_data *data)
{
    long long now;
    t_dongle *left;
    t_dongle *right;

    right = coder->right_dongle;
    left = coder->left_dongle;
    now = ft_get_now_time(data);
    // 1. actualizar disponibilidad
    pthread_mutex_lock(&left->lock_cooldown);
    left->cooldown = now + data->dongle_cooldown;
    pthread_mutex_unlock(&left->lock_cooldown);

    pthread_mutex_lock(&right->lock_cooldown);
    right->cooldown = now + data->dongle_cooldown;
    pthread_mutex_unlock(&right->lock_cooldown);
    // 2. despertar a todos los que están esperando en ambas colas
    pthread_mutex_lock(&left->queue_coders.lock);
    pthread_cond_broadcast(&left->queue_coders.cond);
    pthread_mutex_unlock(&left->queue_coders.lock);

    pthread_mutex_lock(&right->queue_coders.lock);
    pthread_cond_broadcast(&right->queue_coders.cond);
    pthread_mutex_unlock(&right->queue_coders.lock);
}
