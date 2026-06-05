#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "codexion.h"

int ft_is_number(char *str)
{
    int i;

    i = 0;
    if (str[i] == '+')
        i++;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

char *ft_tolower(char *str)
{
    int i;
    int difference;

    i = 0;
    difference = 'a' - 'A';
    while (str[i])
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += difference;
        i++;
    }
    return (str);
}

int ft_valid_scheduler(char *scheduler)
{
    ft_tolower(scheduler);

    if (!strcmp(scheduler, "fifo"))
        return (1);
    if (!strcmp(scheduler, "edf"))
        return (1);
    return (0);
}

int ft_parser_args(char **argv)
{
    int i;

    i = 1;
    if (atoi(argv[1]) <= 0)
            return (0);
    while (i < 8)
    {
        if (!ft_is_number(argv[i]))
            return (0);
        i++;
    }
    if (!ft_valid_scheduler(argv[8]))
            return (0);
    return (1);
}

void ft_generated_data(t_data *data, char **argv)
{
    data->number_coders = atoi(argv[1]);
    data->time_burnout = atoi(argv[2]);
    data->time_compile = atoi(argv[3]);
    data->time_debug = atoi(argv[4]);
    data->time_refactor = atoi(argv[5]);
    data->number_compiles_required = atoi(argv[6]);
    data->dongle_cooldown = atoi(argv[7]);
    data->scheduler = argv[8];
}

void ft_init_data(t_data *data)
{
    data->coders = malloc(sizeof(t_coder) * data->number_coders);
    data->dongles = malloc(sizeof(t_dongle) * data->number_coders);
}

void ft_init_dongles(t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_coders)
    {
        data->dongles[i].number = i;
        data->dongles[i].available = 1;
        i++;
    }
}

void ft_init_coders(t_data *data)
{
    int i;

    i = 0;
    while(i < data->number_coders)
    {
        data->coders[i].coder = i + 1;
        data->coders[i].status = DEBUGGING;
        data->coders[i].compile_count = 0;
        i++;
    }
}

void *routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;

    printf("Coder %d started\n", coder->coder);
    sleep(2);
    printf("Coder %d finished\n", coder->coder);
    return (NULL);
}

int main(int argc, char **argv)
{
    t_data data;

    if (argc == 9)
    {
        if (!ft_parser_args(argv))
        {
            printf("Invalid arguments\n");
            return (1);
        }
        ft_generated_data(&data, argv);
        ft_init_data(&data);
        ft_init_coders(&data);
        ft_init_dongles(&data);

        int i;

        i = 0;
        while (data.number_coders > i)
        {
            pthread_create(
                &data.coders[i].thread, 
                NULL, routine, 
                &data.coders[i]);
            i++;
        }

        i = 0;
        while (data.number_coders > i)
        {
            pthread_join(data.coders[i].thread, NULL);
            i++;
        }
    }
    else{
        printf("Usage: ./codexion ");
        printf("<number_coders> <time_burnout> <time_compile> ");
        printf("<time_debug> <time_refactor> ");
        printf("<number_compiles_required> ");
        printf("<dongle_cooldown> <scheduler>\n");
        printf("[ERROR]: Parameters are missing\n");
    }
    return (0);
}