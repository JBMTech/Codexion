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
    printf("Usage: ./codexion ");
    printf("<number_coders> <time_burnout> <time_compile> ");
    printf("<time_debug> <time_refactor> ");
    printf("<number_compiles_required> ");
    printf("<dongle_cooldown> <scheduler>\n");
    printf("[ERROR]: Parameters are missing\n");
}

void ft_print_log(t_data *data, char *status, int coder_id)
{
    long long time;

    pthread_mutex_lock(&data->lock_print);
    time = get_time_ms() - data->time_start;
    if (strcmp(status, TAKE) == 0)
		printf("[%lld] Coder %d has taken a dongle", time, coder_id);
	else if (strcmp(status, COMP) == 0)
		printf("[%lld] Coder %d is compiling", time, coder_id);
	else if (strcmp(status, DEBUG) == 0)
		printf("[%lld] Coder %d is debugging", time, coder_id);
	else if (strcmp(status, REFACT) == 0)
		printf("[%lld] Coder %d is refactoring", time, coder_id);
	else if (strcmp(status, BURN) == 0)
		printf("[%lld] Coder %d burned out", time, coder_id);
	pthread_mutex_unlock(&data->lock_print);
}
