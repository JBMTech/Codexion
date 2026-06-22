/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:57:13 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/22 10:44:47 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

void	ft_init_mutex_dongle_coder(t_data *data)
{
	int	index;

	index = 0;
	while (data->number_coders != index)
	{
		pthread_mutex_init(&data->coder[index].lock_burnout, NULL);
		pthread_mutex_init(&data->coder[index].lock_finished, NULL);
		pthread_mutex_init(&data->dongle[index].lock, NULL);
		pthread_cond_init(&data->dongle[index].cond, NULL);
		index++;
	}
}

void	ft_init_mutex(t_data *data)
{
	pthread_mutex_init(&data->lock_program, NULL);
	pthread_mutex_init(&data->lock_print, NULL);
	ft_init_mutex_dongle_coder(data);
}
