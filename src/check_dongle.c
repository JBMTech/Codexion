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

int ft_dongle_is_ready(t_dongle *d)
{
    long long now;
    int ready;

    now = ft_get_now_time(d->data);

    pthread_mutex_lock(&d->lock_cooldown);
    ready = (now >= d->cooldown);
    pthread_mutex_unlock(&d->lock_cooldown);

    return ready;
}

void ft_change_cooldown(t_data *data, t_dongle *dongle)
{
    long long now;

    now = ft_get_now_time(data);
    pthread_mutex_lock(&dongle->lock_cooldown);
    dongle->cooldown = now + data->dongle_cooldown;
    pthread_mutex_unlock(&dongle->lock_cooldown);
}

void ft_change_taken(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->lock_taken);
    dongle->taken = 0;
    pthread_mutex_unlock(&dongle->lock_taken);
}

void ft_release_dongles(t_coder *coder, t_data *data)
{
    t_dongle	*left;
    t_dongle	*right;

    left = coder->left_dongle;
    right = coder->right_dongle;

    ft_change_taken(left);
    ft_change_cooldown(data, left);
    ft_change_taken(right);
    ft_change_cooldown(data, right);

    pthread_mutex_lock(&left->queue_coders.lock);
    pthread_cond_broadcast(&left->queue_coders.cond);
    pthread_mutex_unlock(&left->queue_coders.lock);
    
    pthread_mutex_lock(&right->queue_coders.lock);
    pthread_cond_broadcast(&right->queue_coders.cond);
    pthread_mutex_unlock(&right->queue_coders.lock);
}


int ft_take_left_dongle(t_coder *coder)
{
    t_dongle *d;

    d = coder->left_dongle;
    pthread_mutex_lock(&d->lock_taken);
    if (d->taken == 1)
    {
        pthread_mutex_unlock(&d->lock_taken);
        return (0);
    }
    d->taken = 1;
    pthread_mutex_unlock(&d->lock_taken);
    ft_print_log(coder->data, TAKE, coder->coder_id);
    return (1);
}
int ft_take_right_dongle(t_coder *coder)
{
    t_dongle *d;

    d = coder->right_dongle;
    pthread_mutex_lock(&d->lock_taken);
    if (d->taken == 1)
    {
        pthread_mutex_unlock(&d->lock_taken);
        return (0);
    }
    d->taken = 1;
    pthread_mutex_unlock(&d->lock_taken);
    ft_print_log(coder->data, TAKE, coder->coder_id);
    return (1);
}
