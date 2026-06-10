
#include "codexion.h"

void    create_thread(t_data *data)
{
    int index;

    index = 0;
    pthread_create(&data->checker_id, NULL, checker_program, data);
    while (data->number_coders != index)
    {
        pthread_create(&data->coders[index].thread_id, NULL, coder_thread, &data->coders[index]);
        index++;
    }

}