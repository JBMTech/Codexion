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

//----------------------------------------------

int ft_can_take_both(t_coder *c)
{
    t_dongle *left;
    t_dongle *right;
    long long now;

    left = c->left_dongle;
    right = c->right_dongle;

    now = ft_get_now_time(c->data);

    if (!left->queue_coders.first)
        return (0);

    if (!right->queue_coders.first)
        return (0);

    if (left->queue_coders.first->coder != c)
        return (0);

    if (right->queue_coders.first->coder != c)
        return (0);

    if (left->taken)
        return (0);

    if (right->taken)
        return (0);

    if (now < left->cooldown)
        return (0);

    if (now < right->cooldown)
        return (0);

    return (1);
}

void ft_wait_both_dongles(t_coder *c)
{
    t_dongle *l;
    t_dongle *r;

    l = c->left_dongle;
    r = c->right_dongle;;
    while (1)
    {
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
        pthread_cond_wait(&l->cond, &l->lock);
        pthread_mutex_unlock(&l->lock);
        pthread_mutex_unlock(&r->lock);
    }
}

int ft_take_both(t_coder *c)
{
    t_dongle *first;
    t_dongle *second;

    first = c->left_dongle;
    second = c->right_dongle;

    if (first > second)
    {
        first = c->right_dongle;
        second = c->left_dongle;
    }

    pthread_mutex_lock(&first->lock);
    pthread_mutex_lock(&second->lock);

    if (!ft_can_take_both(c))
    {
        pthread_mutex_unlock(&second->lock);
        pthread_mutex_unlock(&first->lock);
        return (0);
    }

    c->left_dongle->taken = 1;
    c->right_dongle->taken = 1;

    ft_remove_from_queue(&c->left_dongle->queue_coders, c);
    ft_remove_from_queue(&c->right_dongle->queue_coders, c);

    pthread_mutex_unlock(&second->lock);
    pthread_mutex_unlock(&first->lock);

    ft_print_log(c->data, TAKE, c->coder_id);
    ft_print_log(c->data, TAKE, c->coder_id);

    return (1);
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


void ft_wait_turn(t_coder *c)
{
    while (ft_get_active_program(c->data))
    {
        if (ft_take_both(c))
            return;

        usleep(100);
    }
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
