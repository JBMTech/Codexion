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

#include "codexion.h"

void *ft_checker_program(void *arg)
{
    t_data  *data;
    int     coder_finish;

    coder_finish = 0;
    data = (t_data *)arg;
    while (ft_get_active_program(data) == 1)
    {
        if (ft_check_burnout(data, &coder_finish) == 1)
            return (NULL);
        if (data->number_coders == coder_finish)
        {
            ft_stop_program(data);
            printf("SUCCESS\n");
            return (NULL);
        }
    }
    return (NULL);
    
}

int ft_check_burnout(t_data *data, int *count)
{
    int index;

    *count = 0;
    index = 0;
    while (data->number_coders != index)
    {
        if (ft_get_active_program(&data->coders[index]) == 1)
            (*count)++;
        if ((ft_get_time_ms() - ft_get_burnout(&data->coders[index])) 
            > data->time_burnout)
        {
            ft_stop_program(data);
            ft_print_log(data, BURN, data->coders->coder_id);
            return (1);
        }
        index++;
    }
    return (0);
}

void ft_stop_program(t_data *data)
{

}
