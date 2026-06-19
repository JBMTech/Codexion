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






void ft_scheduler_fifo(t_coder *coder)
{
    ft_request_dongles(coder);
            while (ft_get_active_program(coder->data))
            {
                if (coder->left_dongle->queue_coders.first == coder &&
                ft_dongle_is_ready(coder->left_dongle))
                    coder->has_left = ft_take_left_dongle(coder);
                if (coder->right_dongle->queue_coders.first == coder &&
                ft_dongle_is_ready(coder->right_dongle))
                    coder->has_right = ft_take_right_dongle(coder);
                if (coder->has_left && coder->has_right)
                    break;
                usleep(8);
            }
            // if (!ft_get_active_program(coder->data))
            //     break ;
            ft_remove_from_dongle_queue(coder);
            ft_compile(coder);
            ft_release_dongles(coder, coder->data);
            ft_debug(coder);
            ft_refract(coder);
}





void *ft_coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while ((ft_get_active_program(coder->data) == 1) && coder->coder_finished == 0)
    {
        if (ft_is_fifo(coder->data))
        {
            if (coder->coder_id % 2)
                usleep(10);
            ft_scheduler_fifo(coder);
        }
    }
    return (NULL);
}
