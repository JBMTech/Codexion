/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   management_time.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:58:21 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:58:22 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long ft_get_time_ms(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long long ft_get_start_time(t_data *data)
{
    return (ft_get_time_ms() - data->time_start);
}
