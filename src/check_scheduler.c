/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_scheduler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:53:37 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/22 13:07:09 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int	ft_is_fifo(t_data *data)
{
	if (strcmp(FIFO, data->scheduler) == 0)
		return (1);
	return (0);
}

int	ft_is_dfe(t_data *data)
{
	if (strcmp(EDF, data->scheduler) == 0)
		return (1);
	return (0);
}

void	ft_scheduler_fifo(t_coder *coder)
{
	if (coder->coder_id % 2 == 0)
		usleep(1000);
	ft_request_dongles(coder);
	ft_wait_turn(coder);
	if (!ft_get_active_program(coder->data))
		return ;
	ft_release_dongles(coder, coder->data);
	ft_debug(coder);
	ft_refract(coder);
}

void	ft_scheduler_edf(t_coder *coder)
{
	if (coder->coder_id % 2 == 0)
		usleep(1000);
	ft_request_dongles_edf(coder);
	ft_wait_turn(coder);
	if (!ft_get_active_program(coder->data))
		return ;
	ft_release_dongles(coder, coder->data);
	ft_debug(coder);
	ft_refract(coder);
}
