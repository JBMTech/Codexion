/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:53:49 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:53:51 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int ft_take_left_dongle(t_coder *coder)
{
    t_dongle *d = coder->left_dongle;

    pthread_mutex_lock(&d->lock_cooldown);

    if (d->taken == 1 || d->queue_coders.first->coder != coder)
    {
        pthread_mutex_unlock(&d->lock_cooldown);
        return (0);
    }

    d->taken = 1;
    pthread_mutex_unlock(&d->lock_cooldown);
    ft_print_log(coder->data, TAKE, coder->coder_id);

    return (1);
}
int ft_take_right_dongle(t_coder *coder)
{
    t_dongle *d = coder->right_dongle;

    pthread_mutex_lock(&d->lock_cooldown);

    if (d->taken == 1 || d->queue_coders.first->coder != coder)
    {
        pthread_mutex_unlock(&d->lock_cooldown);
        return (0);
    }

    d->taken = 1;
    pthread_mutex_unlock(&d->lock_cooldown);
    ft_print_log(coder->data, TAKE, coder->coder_id);

    return (1);
}


void ft_wait_for_turn(t_coder *coder)
{
    while (ft_get_active_program(coder->data))
    {
        if (ft_is_first_both_queue(coder) &&
            ft_dongles_available(coder))
            return;

        usleep(10);
    }
}

void ft_compile(t_coder *coder)
{
    if (ft_get_active_program(coder->data))
    {
        ft_set_burnout(coder);
        ft_print_log(coder->data, COMP, coder->coder_id);
        usleep(coder->data->time_compile * 1000);

        coder->coder_compiled += 1;

        if (coder->coder_compiled >= coder->data->number_compiles_required)
            ft_set_finished(coder);
    }
}

void ft_debug(t_coder *coder)
{
    if (!ft_get_active_program(coder->data))
        return;
    ft_print_log(coder->data, DEBUG, coder->coder_id);
    usleep(coder->data->time_debug * 1000);
    
}

void ft_refract(t_coder *coder)
{
    if (!ft_get_active_program(coder->data))
        return;
    ft_print_log(coder->data, REFACT, coder->coder_id);
    usleep(coder->data->time_debug * 1000);
}

void *ft_coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while ((ft_get_active_program(coder->data) == 1) && coder->coder_finished == 0)
    {
        if (ft_is_fifo(coder->data))
        {
            ft_request_dongles(coder);

            ft_wait_for_turn(coder);

            // intentar tomar progresivamente
            while (!ft_take_left_dongle(coder) ||
                !ft_take_right_dongle(coder))
            {
                usleep(8);
            }

            ft_compile(coder);

            ft_release_dongles(coder, coder->data);

            ft_remove_from_dongle_queue(coder);

            ft_debug(coder);
            ft_refract(coder);
        }
    }
    return (NULL);
}
