
#include "codexion.h"

void inti_mutex_dongle_coder(t_data *data)
{
    int index;

    index = 0;
    while (data->number_coders != index)
    {
        pthread_mutex_init(&data->coders[index].lock_burnout, NULL);
        pthread_mutex_init(&data->dongles[index].lock_cooldown, NULL);
    }

}

void init_mutex(t_data *data)
{
    pthread_mutex_init(&data->lock_program, NULL);
    init_mutex_dongle_coder(&data);
}