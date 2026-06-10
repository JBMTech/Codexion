
#include "codexion.h"

void    *coder_thread(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while ((get_active_program(coder->data) == 1) && coder->coder_finished == 0)
    {
        if (is_fifo(coder->data))
            scheduler_fifo(coder->data, coder, ADD);
        else if (is_dfe(coder->data))
            // scheduler_edf
        life_cycle(coder, COMP);
        if (is_fifo(coder->data))
            scheduler_fifo(coder->data, coder, REMOVE);
        life_cycle(coder, DEBUG);
        life_cycle(coder, REFACT);
    }
    return (NULL);
}

int *life_cycle(t_coder *coder, char *status)
{
    if (get_active_program(coder->data) == 0)
        return (NULL);
    else if (strcmp(status, COMP) == 0)
    {
        set_burnout(coder);
        // Print log
        usleep(coder->data->time_compile * 1000);
        coder->coder_compiled += 1;
        if (coder->coder_compiled >= coder->data->number_compiles_required)
            set_finished(coder);
        release_dongles(coder, coder->data);
    }
    else if (strcmp(status, DEBUG) == 0)
    {
        // Print log
        usleep(coder->data->time_debug * 1000);
    }
    else if (strcmp(status, REFACT) == 0)
    {
        // Print log
        usleep(coder->data->time_refactor * 1000);
    }
    return (0);
}
