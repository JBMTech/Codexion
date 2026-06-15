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

int ft_take_dongle(t_dongle *dongle, t_data *data)
{
    pthread_mutex_lock(&dongle->lock_cooldown);
    if (ft_get_start_time(data) >= dongle->cooldown)
        return (0);
    pthread_mutex_unlock(&dongle->lock_cooldown);
    return (1);
}

int ft_check_take_dongle(t_coder *coder)
{
    if (ft_take_dongle(coder->left_dongle, coder->data) == 0)
    {
        if (ft_take_dongle(coder->right_dongle, coder->data) == 0)
        {
            ft_print_log(coder->data, TAKE, coder->coder_id);
            ft_print_log(coder->data, TAKE, coder->coder_id);
            return (0);
        }
        else if (coder->left_dongle == NULL)
        {
            pthread_mutex_lock(&coder->left_dongle->lock_cooldown);
            return (1);
        }
        else
        {
            pthread_mutex_unlock(&coder->left_dongle->lock_cooldown);
            return (1);
        }
    }
    return (1);
}

void ft_release_dongles(t_coder *coder, t_data *data)
{
    long long time_actual;

    time_actual = ft_get_start_time(data);
    coder->left_dongle->cooldown = time_actual + data->dongle_cooldown;
    if (coder->right_dongle != NULL)
        coder->right_dongle->cooldown = time_actual+ data->dongle_cooldown;
    pthread_mutex_unlock(&coder->left_dongle->lock_cooldown);
    if (coder->right_dongle != NULL)
        pthread_mutex_unlock(&coder->right_dongle->lock_cooldown);
    if (ft_is_fifo(data))
    {
        // Bloqueamos la cola.
		pthread_mutex_lock(&data->queue_coders.lock);

		// Despertamos a TODOS los coders que estén esperando.
		pthread_cond_broadcast(&data->queue_coders.cond);

		// Liberamos el mutex.
		pthread_mutex_unlock(&data->queue_coders.lock);
    }
}