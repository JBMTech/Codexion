
#include "codexion.h"

int get_active_program(t_data *data)
{
    int status;

    status = 0;
    pthread_mutex_lock(&data->lock_program);
    status = data->active_program;
    pthread_mutex_unlock(&data->lock_program);
    return (status);
}

long long get_burnout(t_coder *coder)
{
    long long   burnout;

    burnout = 0;
    pthread_mutex_lock(&coder->lock_burnout);
    burnout = coder->time_burnout;
    pthread_mutex_unlock(&coder->lock_burnout);
    return (burnout);
}