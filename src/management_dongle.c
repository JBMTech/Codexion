/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   management_dongle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 13:05:33 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/22 13:05:37 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int	ft_take_both(t_coder *c)
{
	t_dongle	*first;
	t_dongle	*second;

	first = c->left_dongle;
	second = c->right_dongle;
	if (!second)
		return (0);
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
	if (!ft_can_take_both(c))
	{
		pthread_mutex_unlock(&second->lock);
		pthread_mutex_unlock(&first->lock);
		return (0);
	}
	c->left_dongle->taken = 1;
	c->right_dongle->taken = 1;
	ft_remove_from_queue(&c->left_dongle->queue_coders, c);
	ft_remove_from_queue(&c->right_dongle->queue_coders, c);
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
	return (1);
}

// Reinicia el cooldown
void	ft_change_cooldown(t_data *data, t_dongle *dongle)
{
	long long	now;

	now = ft_get_now_time(data);
	pthread_mutex_lock(&dongle->lock);
	dongle->cooldown = now + data->dongle_cooldown;
	pthread_mutex_unlock(&dongle->lock);
}

// Suelta los dongles
void	ft_change_taken(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->taken = 0;
	pthread_mutex_unlock(&dongle->lock);
}

void	ft_release_dongles(t_coder *coder, t_data *data)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left_dongle;
	right = coder->right_dongle;
	ft_change_taken(left);
	ft_change_cooldown(data, left);
	ft_change_taken(right);
	ft_change_cooldown(data, right);
	pthread_mutex_lock(&left->lock);
	pthread_cond_broadcast(&left->cond);
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_lock(&right->lock);
	pthread_cond_broadcast(&right->cond);
	pthread_mutex_unlock(&right->lock);
}
