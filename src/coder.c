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

#include "codexion.h"

void *ft_coder_thread(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while ((ft_get_active_program(coder->data) == 1) && coder->coder_finished == 0)
    {
        if (ft_is_fifo(coder->data))
            ft_scheduler_fifo(coder->data, coder, ADD);
        else if (is_dfe(coder->data))
            // scheduler_edf
        ft_life_cycle(coder, COMP);
        if (is_fifo(coder->data))
            ft_scheduler_fifo(coder->data, coder, REMOVE);
        ft_life_cycle(coder, DEBUG);
        ft_life_cycle(coder, REFACT);
    }
    return (NULL);
}

int *ft_life_cycle(t_coder *coder, char *status)
{
    if (ft_get_active_program(coder->data) == 0)
        return (NULL);
    else if (strcmp(status, COMP) == 0)
    {
        ft_set_burnout(coder);
        ft_print_log(coder->data, COMP, coder->coder_id);
        usleep(coder->data->time_compile * 1000);
        coder->coder_compiled += 1;
        if (coder->coder_compiled >= coder->data->number_compiles_required)
            ft_set_finished(coder);
        ft_release_dongles(coder, coder->data);
    }
    else if (strcmp(status, DEBUG) == 0)
    {
        ft_print_log(coder->data, DEBUG, coder->coder_id);
        usleep(coder->data->time_debug * 1000);
    }
    else if (strcmp(status, REFACT) == 0)
    {
        ft_print_log(coder->data, REFACT, coder->coder_id);
        usleep(coder->data->time_refactor * 1000);
    }
    return (0);
}
