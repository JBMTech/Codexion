
#include "codexion.h"

void    print_msg(t_data *data, char *action)
{
    long long time;

    pthread_mutex_lock(&data->lock_print);

    time = get_time_ms() - data->time_start;
    if (strcmp(action, TAKE) == 0)
		printf("[%s] ", time, index, dongle_id);


}

void	print_logs(int index, char *dongle_id,
			char *action, t_data *data)
{
	long long	time;

	// Bloquea la impresión para que sólo un hilo
	// pueda escribir en pantalla.
	pthread_mutex_lock(&data->mutex_print);

	// Tiempo transcurrido desde el inicio de la simulación.
	time = get_time_ms() - data->start_time;

	// Si la acción es coger un dongle.
	if (strcmp(action, ACT_TAKE) == 0)
		printf(LOG_TAKE_DONGLE, time, index, dongle_id);

	// Si la acción es compilar.
	else if (strcmp(action, ACT_COMP) == 0)
		printf(LOG_COMPILING, time, index,
			data->coder[index - 1].code_compiled + 1);

	// Si la acción es debug.
	else if (strcmp(action, ACT_DEBUG) == 0)
		printf(LOG_DEBUGGING, time, index);

	// Si la acción es refactor.
	else if (strcmp(action, ACT_REFAC) == 0)
		printf(LOG_REFACTOR, time, index);

	// Si la acción es burnout.
	else if (strcmp(action, ACT_BURNS) == 0)
		printf(LOG_BURNS_OUT, time, index);

	// Permite que otro hilo pueda imprimir.
	pthread_mutex_unlock(&data->mutex_print);
}