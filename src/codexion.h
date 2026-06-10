
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
	int				dongle_id;
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

	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		checker_id;
	pthread_mutex_t	lock_program;
	pthread_mutex_t	lock_print;
	t_queue			queue_coders;
}	t_data;

// utils
int ft_is_number(char *str);
char *ft_tolower(char *str);
int ft_valid_scheduler(char *scheduler);
int ft_parser_args(char **argv);
void ft_generated_data(t_data *data, char **argv);

// init_struct
void ft_init_data(t_data *data);
void ft_init_coders(t_data *data);
void ft_init_dongles(t_data *data);
void ft_assign_dongles(t_data *data);


// getter
int	get_active_program(t_data *data);
long long get_burnout(t_coder *coder);


// init_thread
void	create_thread(t_data *data);


// monitoring
void *checker_program(void *arg);
int check_burnout(t_data *data);

// coder
void    *coder_thread(void *arg);

// check_schuduler
int is_fifo(t_data *data);
int is_dfe(t_data *data);

// create_mutex
void inti_mutex_dongle_coder(t_data *data);
void init_mutex(t_data *data);

// management_time
long long get_time_ms(void);
long long get_start_time(t_data *data);

// management_queue
int add_to_queue(t_coder *coder_to_add, t_queue *queue);
int remove_from_queue(t_queue *queue);
void fifo_manager_queue(t_data *data, t_coder *coder);
int scheduler_fifo(t_data *data, t_coder *coder, char *action);


// setter
void set_burnout(t_coder *coder);
void set_finished(t_coder *coder);

#endif