/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabuleje <jabuleje@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 11:54:05 by jabuleje          #+#    #+#             */
/*   Updated: 2026/06/11 11:54:07 by jabuleje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

int main(int argc, char **argv)
{
    t_data data;

    if (argc != 9)
        ft_print_msg();
    if (!ft_parser_args(argv))
    {
        printf("[ERROR]: Invalid arguments\n");
        return (1);
    }
    memset(&data, 0, sizeof(t_data));
    if (!ft_generated_data(&data, argv))
    {
        printf("[ERROR]: malloc error\n");
        return (1);
    }
    ft_init_data(&data);
    ft_init_mutex(&data);
    ft_create_thread(&data);
    ft_join_thread(&data);

    return (0);
}
