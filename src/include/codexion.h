/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:54:23 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:54:27 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


// Scheduler //
# define FIFO		"fifo"
# define EDF		"edf"

// Action //
# define ADD		"add_queue"
# define REMOVE		"remove_queue"
# define TAKE		"take_dongle"

// Status //
# define COMP		"COMPILING"
# define DEBUG		"DEBUGGING"
# define REFACT		"REFACTORING"
# define BURN		"BURNED_OUT"

typedef struct s_data			t_data;
typedef struct s_coder			t_coder;
typedef struct s_dongle			t_dongle;
typedef struct s_node_coder		t_node_coder;
typedef struct s_queue			t_queue;


typedef struct s_nodo_coder
{
	t_coder			*coder;
	struct s_nodo_coder	*next;
}	t_nodo_coder;

typedef struct s_queue
{
	t_nodo_coder		*first;
	t_nodo_coder		*last;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
}	t_queue;

typedef struct s_dongle
{
	long long		cooldown;
	t_data			*data;
	pthread_mutex_t	lock_cooldown;
}	t_dongle;

typedef struct s_coder{
    int				coder_id;
	int				coder_compiled;
	int				coder_finished;
	long long		time_burnout;

	t_data			*data;
    t_dongle		*left_dongle;
    t_dongle		*right_dongle;

	pthread_t		thread_id;
	pthread_mutex_t	lock_burnout;
	pthread_mutex_t lock_finished;
}   t_coder;

typedef struct s_data
{
	int				number_coders;
	long long		time_burnout;
	long long		time_compile;
	long long		time_debug;
	long long		time_refactor;
	long long		time_start;
	int				active_program;
	int				number_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;

	t_coder			*coder;
	t_dongle		*dongle;
	pthread_t		checker_id;
	pthread_mutex_t	lock_program;
	pthread_mutex_t	lock_print;
	t_queue			queue_coders;
}	t_data;

// init_parser
int ft_is_number(char *str);
char *ft_tolower(char *str);
int ft_valid_scheduler(char *scheduler);
int ft_parser_args(char **argv);
int ft_generated_data(t_data *data, char **argv);

//

// getter
int	ft_get_active_program(t_data *data);
long long ft_get_burnout(t_coder *coder);
int	ft_get_have_finished(t_coder *coder);

// init_thread
void ft_create_thread(t_data *data);
void ft_join_thread(t_data *data);

// monitoring
void *ft_checker_program(void *arg);
int ft_check_burnout(t_data *data, int *finished);
void ft_stop_program(t_data *data);

// coder
void *ft_coder_thread(void *arg);
int *ft_life_cycle(t_coder *coder, char *status);

// check_schuduler
int ft_is_fifo(t_data *data);
int ft_is_dfe(t_data *data);

// check_dongle
int ft_take_dongle(t_dongle *dongle, t_data *data);
int ft_check_take_dongle(t_coder *coder);
void ft_release_dongles(t_coder *coder, t_data *data);

// create_mutex
void ft_init_mutex_dongle_coder(t_data *data);
void ft_init_mutex(t_data *data);

// management_time
long long ft_get_time_ms(void);
long long ft_get_start_time(t_data *data);

// management_queue
int ft_add_to_queue(t_coder *coder_to_add, t_queue *queue);
int ft_remove_from_queue(t_queue *queue);
void ft_fifo_manager_queue(t_data *data, t_coder *coder);
int ft_scheduler_fifo(t_data *data, t_coder *coder, char *action);

// setter
void ft_set_burnout(t_coder *coder);
void ft_set_finished(t_coder *coder);

// log
void ft_print_msg(void);
void ft_print_log(t_data *data, char *status, int coder_id);

// init_proyect
void ft_init_data(t_data *data);
void ft_create_coders_and_dongles(t_data *data);
void ft_init_coders(t_data *data, t_coder *coder, int count);
void ft_init_basic_data_coders(t_data *data, t_coder *coder, int nbr_coder);

#endif
