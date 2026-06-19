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
// Cumprueba si ha terminado el cooldown de dongle
int ft_dongle_is_ready(t_dongle *d)
{
    long long now;
    int ready;

    now = ft_get_now_time(d->data);

    pthread_mutex_lock(&d->lock);
    ready = (now >= d->cooldown);
    pthread_mutex_unlock(&d->lock);

    return ready;
}

// Reinicia el cooldown
void ft_change_cooldown(t_data *data, t_dongle *dongle)
{
    long long now;

    now = ft_get_now_time(data);
    pthread_mutex_lock(&dongle->lock);
    dongle->cooldown = now + data->dongle_cooldown;
    pthread_mutex_unlock(&dongle->lock);
}

// Suelta los dongles
void ft_change_taken(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->lock);
    dongle->taken = 0;
    pthread_mutex_unlock(&dongle->lock);
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

    pthread_mutex_lock(&left->lock);
    pthread_cond_broadcast(&left->cond);
    pthread_mutex_unlock(&left->lock);
    
    pthread_mutex_lock(&right->lock);
    pthread_cond_broadcast(&right->cond);
    pthread_mutex_unlock(&right->lock);
}

// El coder toma dongle izquierdo
int ft_take_left_dongle(t_coder *coder)
{
    t_dongle *d;

    d = coder->left_dongle;
    pthread_mutex_lock(&d->lock);
    if (d->taken == 1)
    {
        pthread_mutex_unlock(&d->lock);
        return (0);
    }
    d->taken = 1;
    pthread_mutex_unlock(&d->lock);
    ft_print_log(coder->data, TAKE, coder->coder_id);
    return (1);
}

//El coder toma dongle derecho
int ft_take_right_dongle(t_coder *coder)
{
    t_dongle *d;

    d = coder->right_dongle;
    pthread_mutex_lock(&d->lock);
    if (d->taken == 1)
    {
        pthread_mutex_unlock(&d->lock);
        return (0);
    }
    d->taken = 1;
    pthread_mutex_unlock(&d->lock);
    ft_print_log(coder->data, TAKE, coder->coder_id);
    return (1);
}
