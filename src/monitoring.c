
#include "codexion.h"

void *checker_program(void *arg)
{
    t_data  *data;
    int     coder_finish;

    coder_finish = 0;
    data = (t_data *)arg;
    while (get_active_program(data) == 1)
    {
        if (check_burnout(data, &coder_finish) == 1)
            return (NULL);
        if (data->number_coders == coder_finish)
        {
            stop_program(data);
            printf("SUCCESS\n");
            return (NULL);
        }
    }
    return (NULL);
    
}

int check_burnout(t_data *data, int *count)
{
    int index;

    *count = 0;
    index = 0;
    while (data->number_coders != index)
    {
        if (get_active_program(&data->coders[index]) == 1)
            (*count)++;
        if ((get_time_ms() - get_burnout(&data->coders[index])) 
            > data->time_burnout)
        {
            // Detener el programa
            // Imprimir mensaje
            return (1);
        }
        index++;
    }
    return (0);
}

void stop_program(t_data *data)
{

}
