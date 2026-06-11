/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_mutex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:57:13 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:57:14 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void ft_inti_mutex_dongle_coder(t_data *data)
{
    int index;

    index = 0;
    while (data->number_coders != index)
    {
        pthread_mutex_init(&data->coders[index].lock_burnout, NULL);
        pthread_mutex_init(&data->dongles[index].lock_cooldown, NULL);
    }
}

void ft_init_mutex(t_data *data)
{
    pthread_mutex_init(&data->lock_program, NULL);
    init_mutex_dongle_coder(&data);
}
