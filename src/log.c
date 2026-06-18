/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:59:33 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:59:36 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

void ft_print_msg(void) 
 {
    printf(RED "[ERROR]: Parameters are missing\n" RESET);
    printf(GREEN "Usage: ./codexion " RESET);
    printf(GREEN "<number_coders> <time_burnout> <time_compile> " RESET);
    printf(GREEN "<time_debug> <time_refactor> " RESET);
    printf(GREEN "<number_compiles_required> " RESET);
    printf(GREEN "<dongle_cooldown> <scheduler>\n" RESET);
}

void ft_print_log(t_data *data, char *status, int coder_id)
{
    long long time;

    pthread_mutex_lock(&data->lock_print);
    time = ft_get_time_ms() - data->time_start;
    if (strcmp(status, TAKE) == 0)
		printf(BLUE "[%lld] Coder %d has taken a dongle\n" RESET, time, coder_id);
	else if (strcmp(status, COMP) == 0)
		printf(GREEN "[%lld] Coder %d is compiling\n" RESET, time, coder_id);
	else if (strcmp(status, DEBUG) == 0)
		printf(YELLOW "[%lld] Coder %d is debugging\n" RESET, time, coder_id);
	else if (strcmp(status, REFACT) == 0)
		printf(PURPLE "[%lld] Coder %d is refactoring\n" RESET, time, coder_id);
	else if (strcmp(status, BURN) == 0)
		printf(RED "[%lld] Coder %d burned out\n" RESET, time, coder_id);
	pthread_mutex_unlock(&data->lock_print);
}
