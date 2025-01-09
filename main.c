/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:00:47 by hallfana          #+#    #+#             */
/*   Updated: 2025/01/09 23:47:45 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc != 5 && argc != 6)
		error_exit("wrong input\n\t./philosophers 5 750 250 250 [5]\n");
	parse_argv(&table, argv);
	init(&table);
	dinner(&table);
	clean(&table);
	return (0);
}
