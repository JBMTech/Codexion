/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:57:26 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/22 11:29:32 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int	ft_get_active_program(t_data *data)
{
	int	status;

	status = 0;
	pthread_mutex_lock(&data->lock_program);
	status = data->active_program;
	pthread_mutex_unlock(&data->lock_program);
	return (status);
}

long long	ft_get_burnout(t_coder *coder)
{
	long long	burnout;

	burnout = 0;
	pthread_mutex_lock(&coder->lock_burnout);
	burnout = coder->time_burnout;
	pthread_mutex_unlock(&coder->lock_burnout);
	return (burnout);
}

int	ft_get_have_finished(t_coder *coder)
{
	int	finished;

	finished = 0;
	pthread_mutex_lock(&coder->lock_finished);
	finished = coder->coder_finished;
	pthread_mutex_unlock(&coder->lock_finished);
	return (finished);
}

void	ft_request_dongles_dfe(t_coder *coder)
{
	
	if (coder->coder_id % 2 != 0)
	{
		if (coder->left_dongle)
			ft_queue_access(coder, coder->left_dongle);
		if (coder->right_dongle)
			ft_queue_access(coder, coder->right_dongle);
	}
	else if (coder->coder_id % 2 == 0)
	{
		usleep(9);
		if (coder->right_dongle)
			ft_queue_access(coder, coder->right_dongle);
		if (coder->left_dongle)
			ft_queue_access(coder, coder->left_dongle);
	}
}

// int	ft_coder_near_deadline(t_dongle *dongle)
// {
// 	long long	deadline;

// 	deadline = ft_get_burnout(coder) + coder->last_compile_start;
// 	// En left_dongle
// 	// Si el coder.fisrt es menor que coder.second
// 		//Gana coder.first --> return (1)
// 	// else -> return (0)
// }
