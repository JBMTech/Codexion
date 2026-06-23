/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:54:23 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/22 11:00:50 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

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

// Colours //
# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define PURPLE  "\033[35m"

typedef struct s_data			t_data;
typedef struct s_coder			t_coder;
typedef struct s_dongle			t_dongle;
typedef struct s_node_coder		t_node_coder;
typedef struct s_queue			t_queue;

typedef struct s_nodo_coder
{
	t_coder				*coder;
	struct s_nodo_coder	*next;
}	t_nodo_coder;

typedef struct s_queue
{
	t_nodo_coder	*first;
	t_nodo_coder	*last;
}	t_queue;

typedef struct s_dongle
{
	long long		cooldown;
	int				taken;
	t_data			*data;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	t_queue			queue_coders;
}	t_dongle;

typedef struct s_coder
{
	int				coder_id;
	int				coder_compiled;
	int				coder_finished;
	long long		time_burnout;
	long long		last_compile_start;
	t_data			*data;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_t		thread_id;
	pthread_mutex_t	lock_burnout;
	pthread_mutex_t	lock_finished;
}	t_coder;

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
}	t_data;

// check_dongle

int			ft_can_take_both(t_coder *c);
void		ft_wait_both_dongles(t_coder *c);
void		ft_wait_turn(t_coder *c);
void		ft_request_dongles(t_coder *coder);
void		ft_request_dongles_dfe(t_coder *coder);

// check_schuduler
int			ft_is_fifo(t_data *data);
int			ft_is_dfe(t_data *data);
void		ft_scheduler_fifo(t_coder *coder);
void		ft_scheduler_dfe(coder);

// coder
void		*ft_coder_routine(void *arg);
void		ft_compile(t_coder *coder);
void		ft_debug(t_coder *coder);
void		ft_refract(t_coder *coder);

// getter
int			ft_get_active_program(t_data *data);
long long	ft_get_burnout(t_coder *coder);
int			ft_get_have_finished(t_coder *coder);
long long	ft_get_coder_deadline(t_coder *coder);

// init_mutex
void		ft_init_mutex_dongle_coder(t_data *data);
void		ft_init_mutex(t_data *data);

// init_parser
int			ft_is_number(char *str);
char		*ft_tolower(char *str);
int			ft_valid_scheduler(char *scheduler);
int			ft_parser_args(char **argv);
int			ft_generated_data(t_data *data, char **argv);

// init_proyect
void		ft_init_data(t_data *data);
void		ft_create_coders_and_dongles(t_data *data);
void		ft_init_coders(t_data *data, t_coder *coder, int count);
void		ft_init_basic_data_coders(t_data *data, t_coder *coder,
				int nbr_coder);

// init_thread
void		ft_create_thread(t_data *data);
void		ft_join_thread(t_data *data);

// log
void		ft_print_msg(void);
void		ft_print_log(t_data *data, char *status, int coder_id);

// management_dongle
int			ft_take_both(t_coder *c);
void		ft_change_cooldown(t_data *data, t_dongle *dongle);
void		ft_change_taken(t_dongle *dongle);
void		ft_release_dongles(t_coder *coder, t_data *data);

// management_queue
void		ft_queue_access(t_coder *coder, t_dongle *dongle);
int			ft_add_to_queue(t_coder *coder_to_add, t_queue *queue);
void		ft_queue_access_dfe(t_coder *coder, t_dongle *dongle);
void		ft_remove_from_queue(t_queue *queue, t_coder *coder);
void		ft_remove_from_dongle_queue(t_coder *coder);

// management_time
long long	ft_get_time_ms(void);
long long	ft_get_now_time(t_data *data);

// monitoring
void		*ft_checker_program(void *arg);
int			ft_check_burnout(t_data *data);
void		ft_stop_program(t_data *data);
void		ft_get_nbr_coder_finished(t_data *data, int *finished);

// setter
void		ft_set_burnout(t_coder *coder);
void		ft_set_finished(t_coder *coder);

// queue_dfe
int	ft_add_to_queue_edf(t_coder *coder, t_queue *queue);

#endif
