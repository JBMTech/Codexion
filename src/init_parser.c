/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:58:58 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/22 10:29:56 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int	ft_is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_valid_scheduler(char *scheduler)
{
	if (!strcmp(scheduler, "fifo"))
		return (1);
	if (!strcmp(scheduler, "edf"))
		return (1);
	return (0);
}

int	ft_parser_args(char **argv)
{
	int	i;

	i = 1;
	if (atoi(argv[1]) <= 0)
		return (0);
	while (i < 8)
	{
		if (!ft_is_number(argv[i]))
			return (0);
		i++;
	}
	if (!ft_valid_scheduler(argv[8]))
		return (0);
	return (1);
}

int	ft_generated_data(t_data *data, char **argv)
{
	int	n;

	n = atoi(argv[1]);
	data->coder = malloc(sizeof(t_coder) * n);
	if (!data->coder)
		return (0);
	data->dongle = malloc(sizeof(t_dongle) * n);
	if (!data->dongle)
	{
		free(data->coder);
		return (0);
	}
	data->number_coders = n;
	data->time_burnout = atoi(argv[2]);
	data->time_compile = atoi(argv[3]);
	data->time_debug = atoi(argv[4]);
	data->time_refactor = atoi(argv[5]);
	data->number_compiles_required = atoi(argv[6]);
	data->dongle_cooldown = atoi(argv[7]);
	data->scheduler = argv[8];
	return (1);
}
