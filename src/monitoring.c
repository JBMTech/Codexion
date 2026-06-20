/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:58:31 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:58:32 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

void *ft_checker_program(void *arg)
{
    t_data  *data;
    int     coder_finish;

    data = (t_data *)arg;
    while (ft_get_active_program(data) == 1)
    {
        ft_get_nbr_coder_finished(data, &coder_finish);
        if (data->number_coders == coder_finish)
        {
            ft_stop_program(data);
            pthread_mutex_lock(&data->lock_print);
            printf("SUCCESS\n");
            pthread_mutex_unlock(&data->lock_print);
            return (NULL);
        }
        if (ft_check_burnout(data) == 1)
            return (NULL);
    }
    return (NULL);
}

void ft_get_nbr_coder_finished(t_data *data, int *finished)
{
    int index;

    *finished = 0;
    index = 0;
    while (data->number_coders != index)
    {
        if (ft_get_have_finished(&data->coder[index]) == 1)
            (*finished)++;
        index++;
    }
}

int ft_check_burnout(t_data *data)
{
    int index;

    index = 0;
    while (data->number_coders != index)
    {
        if ((ft_get_time_ms() - ft_get_burnout(&data->coder[index]))
            > data->time_burnout)
        {
            ft_stop_program(data);
            ft_print_log(data, BURN, data->coder[index].coder_id);
            return (1);
        }
        index++;
    }
    return (0);
}

void ft_stop_program(t_data *data)
{
    int i;

    pthread_mutex_lock(&data->lock_program);
    data->active_program = 0;
    pthread_mutex_unlock(&data->lock_program);

    i = 0;
    while (i < data->number_coders)
    {
        pthread_mutex_lock(&data->dongle[i].lock);
        pthread_cond_broadcast(&data->dongle[i].cond);
        pthread_mutex_unlock(&data->dongle[i].lock);
        i++;
    }
}
