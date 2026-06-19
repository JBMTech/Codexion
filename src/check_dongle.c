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

// Añadir a la cola de los dongles 
void ft_request_dongles(t_coder *coder)
{
    pthread_mutex_lock(&coder->left_dongle->lock);
    ft_add_to_queue(coder, &coder->left_dongle->queue_coders);
    pthread_mutex_unlock(&coder->left_dongle->lock);
    
    pthread_mutex_lock(&coder->right_dongle->lock);
    ft_add_to_queue(coder, &coder->right_dongle->queue_coders);
    pthread_mutex_unlock(&coder->right_dongle->lock);
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

int ft_can_take_dongle(t_coder *coder, t_dongle *d)
{
    long long now;

    now = ft_get_now_time(d->data);

    pthread_mutex_lock(&d->lock);
    if (!d->queue_coders.first)
        return (0);
    pthread_mutex_unlock(&d->lock);

    if (d->queue_coders.first->coder != coder)
        return (0);

    if (d->taken)
        return (0);

    if (now < d->cooldown)
        return (0);

    return (1);
}


void ft_wait_dongle(t_coder *coder, t_dongle *d)
{
    pthread_mutex_lock(&d->lock);

    while (!ft_can_take_dongle(coder, d)
        && ft_get_active_program(coder->data))
    {
        pthread_cond_wait(&d->cond, &d->lock);
    }

    pthread_mutex_unlock(&d->lock);
}

void ft_release_dongle(t_dongle *d)
{
    long long now;

    pthread_mutex_lock(&d->lock);

    now = ft_get_now_time(d->data);

    d->taken = 0;
    d->cooldown = now + d->data->dongle_cooldown;

    pthread_cond_broadcast(&d->cond);

    pthread_mutex_unlock(&d->lock);
}
//----------------------------------------------

int ft_can_take_both(t_coder *c)
{
    t_dongle *l = c->left_dongle;
    t_dongle *r = c->right_dongle;
    long long now = ft_get_now_time(c->data);

    if (!l->queue_coders.first || l->queue_coders.first->coder != c)
        return (0);

    if (!r->queue_coders.first || r->queue_coders.first->coder != c)
        return (0);

    if (l->taken || r->taken)
        return (0);

    if (now < l->cooldown || now < r->cooldown)
        return (0);

    return (1);
}

void ft_wait_both_dongles(t_coder *c)
{
    t_dongle *l = c->left_dongle;
    t_dongle *r = c->right_dongle;

    while (1)
    {
        // orden global evita deadlock
        if (l < r)
        {
            pthread_mutex_lock(&l->lock);
            pthread_mutex_lock(&r->lock);
        }
        else
        {
            pthread_mutex_lock(&r->lock);
            pthread_mutex_lock(&l->lock);
        }

        if (ft_can_take_both(c))
        {
            pthread_mutex_unlock(&l->lock);
            pthread_mutex_unlock(&r->lock);
            return;
        }

        // esperar en UNO (pero con condición global)
        pthread_cond_wait(&l->cond, &l->lock);

        pthread_mutex_unlock(&l->lock);
        pthread_mutex_unlock(&r->lock);
    }
}

void ft_take_both(t_coder *c)
{
    t_dongle *l = c->left_dongle;
    t_dongle *r = c->right_dongle;

    pthread_mutex_lock(&l->lock);
    pthread_mutex_lock(&r->lock);

    l->taken = 1;
    r->taken = 1;

    ft_remove_from_queue(&l->queue_coders, c);
    ft_remove_from_queue(&r->queue_coders, c);

    pthread_mutex_unlock(&r->lock);
    pthread_mutex_unlock(&l->lock);

    ft_print_log(c->data, TAKE, c->coder_id);
    ft_print_log(c->data, TAKE, c->coder_id);
}


void ft_release_both(t_coder *c)
{
    t_dongle *l = c->left_dongle;
    t_dongle *r = c->right_dongle;
    long long now = ft_get_now_time(c->data);

    pthread_mutex_lock(&l->lock);
    l->taken = 0;
    l->cooldown = now + c->data->dongle_cooldown;
    pthread_cond_broadcast(&l->cond);
    pthread_mutex_unlock(&l->lock);

    pthread_mutex_lock(&r->lock);
    r->taken = 0;
    r->cooldown = now + c->data->dongle_cooldown;
    pthread_cond_broadcast(&r->cond);
    pthread_mutex_unlock(&r->lock);
}


void ft_wait_dongle(t_dongle *d, t_coder *c)
{
    pthread_mutex_lock(&d->lock);

    while (1)
    {
        if (d->queue_coders.first &&
            d->queue_coders.first->coder == c &&
            d->taken == 0 &&
            ft_get_now_time(d->data) >= d->cooldown &&
            ft_get_active_program(d->data))
            break;

        pthread_cond_wait(&d->cond, &d->lock);
    }

    pthread_mutex_unlock(&d->lock);
}


int ft_take_dongle(t_dongle *d, t_coder *c)
{
    pthread_mutex_lock(&d->lock);

    if (!d->queue_coders.first ||
        d->queue_coders.first->coder != c ||
        d->taken ||
        ft_get_now_time(d->data) < d->cooldown)
    {
        pthread_mutex_unlock(&d->lock);
        return 0;
    }

    d->taken = 1;
    ft_remove_from_queue(&d->queue_coders, c);

    pthread_mutex_unlock(&d->lock);

    ft_print_log(c->data, TAKE, c->coder_id);
    return 1;
}
