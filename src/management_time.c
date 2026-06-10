
#include "codexion.h"

long long get_time_ms(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long long get_start_time(t_data *data)
{
    return (get_time_ms() - data->time_start);
}