/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dongle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:53:13 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/22 13:08:32 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int	ft_can_take_both(t_coder *c)
{
	t_dongle	*left;
	t_dongle	*right;
	long long	now;

	left = c->left_dongle;
	right = c->right_dongle;
	now = ft_get_now_time(c->data);
	if (!left->queue_coders.first)
		return (0);
	if (!right->queue_coders.first)
		return (0);
	if (left->queue_coders.first->coder != c)
		return (0);
	if (right->queue_coders.first->coder != c)
		return (0);
	if (left->taken)
		return (0);
	if (right->taken)
		return (0);
	if (now < left->cooldown)
		return (0);
	if (now < right->cooldown)
		return (0);
	return (1);
}

void	ft_wait_both_dongles(t_coder *c)
{
	while (1)
	{
		if (ft_can_take_both(c))
		{
			pthread_mutex_unlock(&c->left_dongle->lock);
			pthread_mutex_unlock(&c->right_dongle->lock);
			return ;
		}
		pthread_cond_wait(&c->left_dongle->cond, &c->left_dongle->lock);
		pthread_mutex_unlock(&c->left_dongle->lock);
		pthread_mutex_unlock(&c->right_dongle->lock);
	}
}

void	ft_wait_turn(t_coder *c)
{
	while (ft_get_active_program(c->data))
	{
		if (ft_take_both(c))
		{
			ft_print_log(c->data, TAKE, c->coder_id);
			ft_print_log(c->data, TAKE, c->coder_id);
			ft_compile(c);
			return ;
		}
		usleep(9);
	}
}
