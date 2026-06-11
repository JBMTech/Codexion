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

#include "codexion.h"

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
    ft_generated_data(&data, argv);
    ft_inti_mutex_dongle_coder(&data);
    ft_create_thread(&data);

    return (0);
}
