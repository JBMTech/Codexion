/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:58:47 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:58:48 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void ft_set_burnout(t_coder *coder)
{
    pthread_mutex_lock(&coder->lock_burnout);
    coder->time_burnout = get_time_ms();
    pthread_mutex_unlock(&coder->lock_burnout);
}

void ft_set_finished(t_coder *coder)
{
    pthread_mutex_lock(&coder->lock_finished);
    coder->coder_finished = 1;
    pthread_mutex_unlock(&coder->lock_finished);
}
