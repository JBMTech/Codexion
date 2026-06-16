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

#include "include/codexion.h"

void ft_create_thread(t_data *data)
{
    int index;

    index = 0;
    pthread_create(&data->checker_id, NULL, ft_checker_program, data);
    while (data->number_coders != index)
    {
        pthread_create(&data->coder[index].thread_id, NULL, ft_coder_routine, &data->coder[index]);
        index++;
    }
}

void    ft_join_thread(t_data *data)
{
	int	index;

	index = 0;
	pthread_join(data->checker_id, NULL);
	while (index != data->number_coders)
	{
		pthread_join(data->coder[index].thread_id, NULL);
		index++;
	}
}
