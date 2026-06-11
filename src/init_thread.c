/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:57:43 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:57:44 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void ft_create_thread(t_data *data)
{
    int index;

    index = 0;
    pthread_create(&data->checker_id, NULL, ft_checker_program, data);
    while (data->number_coders != index)
    {
        pthread_create(&data->coders[index].thread_id, NULL, ft_coder_thread, &data->coders[index]);
        index++;
    }
}
