/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_proyect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 10:40:26 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/15 10:40:29 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"


void ft_init_data(t_data *data)
{
    data->active_program = 1;
	data->time_start = ft_get_time_ms();
	data->dongle->queue_coders.first = NULL;
	data->dongle->queue_coders.last = NULL;
	ft_create_coders_and_dongles(data);
}

void ft_create_coders_and_dongles(t_data *data)
{
	int index;

	index = 0;
	while (data->number_coders != index)
	{
		ft_init_coders(data, &data->coder[index], index);
		data->dongle[index].cooldown = 0;
		data->dongle[index].data = data;
		index++;
	}
}

void ft_init_coders(t_data *data, t_coder *coder, int count)
{
	int index;

	ft_init_basic_data_coders(data, coder, count);
	if (data->number_coders > 1)
	{
		index = (count + 1) % data->number_coders;
		if (count < index)
		{
			coder->left_dongle = &data->dongle[count];
			coder->right_dongle = &data->dongle[index];
		}
		else
		{
			coder->left_dongle = &data->dongle[index];
			coder->right_dongle = &data->dongle[count];
		}
	}
}

void ft_init_basic_data_coders(t_data *data, t_coder *coder, int nbr_coder)
{
	coder->coder_id = nbr_coder + 1;
	coder->time_burnout = ft_get_time_ms();
	coder->coder_compiled = 0;
	coder->coder_finished = 0;
	coder->data = data;
	coder->left_dongle = NULL;
	coder->right_dongle = NULL;
}
