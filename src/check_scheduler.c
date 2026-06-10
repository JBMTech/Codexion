
#include "codexion.h"

int is_fifo(t_data *data)
{
    if (strcmp(FIFO, data->scheduler) == 0)
        return (1);
    return (0);
}

int is_dfe(t_data *data)
{
    if (strcmp(EDF, data->scheduler) == 0)
        return (1);
    return (0);
}

