
#include "codexion.h"

void    set_burnout(t_coder *coder)
{
    pthread_mutex_lock(&coder->lock_burnout);
    coder->time_burnout = get_time_ms();
    pthread_mutex_unlock(&coder->lock_burnout);
}

void    set_finished(t_coder *coder)
{
    pthread_mutex_lock(&coder->lock_finished);
    coder->coder_finished = 1;
    pthread_mutex_unlock(&coder->lock_finished);
}
