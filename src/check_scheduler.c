/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_scheduler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:53:37 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:53:39 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int ft_is_fifo(t_data *data)
{
    if (strcmp(FIFO, data->scheduler) == 0)
        return (1);
    return (0);
}

int ft_is_dfe(t_data *data)
{
    if (strcmp(EDF, data->scheduler) == 0)
        return (1);
    return (0);
}