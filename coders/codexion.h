
#ifndef CODEXION_H
# define CODEXION_H

typedef struct s_dongle
{
	int				number;
	int				available;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_coder{
    int			coder;
    int			status;
	int			compile_count;

    t_dongle	*left;
    t_dongle	*right;

	pthread_t	thread;
}   t_coder;

typedef struct s_data
{
	int			number_coders;
	int			time_burnout;
	int			time_compile;
	int			time_debug;
	int			time_refactor;
	int			number_compiles_required;
	int			dongle_cooldown;
	char		*scheduler;

	t_coder		*coders;
	t_dongle	*dongles;
}	t_data;

enum e_status
{
    COMPILING,
    DEBUGGING,
    REFACTORING,
    BURNED_OUT
};

#endif