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
    if (now < coder->left_dongle->cooldown && now < coder->right_dongle->cooldown)
    {
        printf("[%lld] left\n", coder->left_dongle->cooldown);
        printf("[%lld] right\n", coder->right_dongle->cooldown);
        printf("[%lld] now_left\n", now);
        printf("[%lld] now_right\n", now);
        return (0);
    }
    return (1);
}

void ft_release_dongles(t_coder *coder, t_data *data)
{
    long long	now;
    t_dongle	*left;
    t_dongle	*right;

    left = coder->left_dongle;
    right = coder->right_dongle;
    now = ft_get_now_time(data);

    pthread_mutex_lock(&left->lock_cooldown);
    left->taken = 0;
    left->cooldown = now + data->dongle_cooldown;
    pthread_mutex_unlock(&left->lock_cooldown);

    pthread_mutex_lock(&right->lock_cooldown);
    right->taken = 0;
    right->cooldown = now + data->dongle_cooldown;
    pthread_mutex_unlock(&right->lock_cooldown);

    pthread_mutex_lock(&left->queue_coders.lock);
    pthread_cond_broadcast(&left->queue_coders.cond);
    pthread_mutex_unlock(&left->queue_coders.lock);

    pthread_mutex_lock(&right->queue_coders.lock);
    pthread_cond_broadcast(&right->queue_coders.cond);
    pthread_mutex_unlock(&right->queue_coders.lock);
}
