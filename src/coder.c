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



int ft_wait_for_turn(t_coder *coder)
{
    while (ft_get_active_program(coder->data))
    {
        if (ft_is_first_both_queue(coder) &&
            ft_dongles_available(coder))
            return (1);
    }
    return (0);
}

void ft_compile(t_coder *coder)
{
    ft_set_burnout(coder);
    ft_print_log(coder->data, COMP, coder->coder_id);
    usleep(coder->data->time_compile * 1000);

    coder->coder_compiled += 1;

    if (coder->coder_compiled >= coder->data->number_compiles_required)
        ft_set_finished(coder);
}

void ft_debug(t_coder *coder)
{
    ft_print_log(coder->data, DEBUG, coder->coder_id);
    usleep(coder->data->time_debug * 1000);
}

void ft_refract(t_coder *coder)
{
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
            if (ft_wait_for_turn(coder))
            {
                ft_print_log(coder->data, TAKE, coder->coder_id);
                ft_print_log(coder->data, TAKE, coder->coder_id);
                ft_compile(coder);
            }
            ft_release_dongles(coder, coder->data);
            ft_remove_from_dongle_queue(coder);
            ft_debug(coder);
            ft_refract(coder);
        }
    }
    return (NULL);
}
