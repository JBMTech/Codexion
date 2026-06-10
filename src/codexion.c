
#include "codexion.h"

int main(int argc, char **argv)
{
    t_data data;

    if (argc != 9)
        print_man();
    if (!ft_parser_args(argv))
    {
        printf("[ERROR]: Invalid arguments\n");
        return (1);
    }
    ft_generated_data(&data, argv);
    ft_init_data(&data);
    ft_init_coders(&data);
    ft_init_dongles(&data);
    ft_assign_dongles(&data);

    return (0);
}


 void   print_man(void) 
 {
    printf("Usage: ./codexion ");
    printf("<number_coders> <time_burnout> <time_compile> ");
    printf("<time_debug> <time_refactor> ");
    printf("<number_compiles_required> ");
    printf("<dongle_cooldown> <scheduler>\n");
    printf("[ERROR]: Parameters are missing\n");
}