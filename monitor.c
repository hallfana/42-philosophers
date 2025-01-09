/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:29:08 by samberna          #+#    #+#             */
/*   Updated: 2025/01/09 23:48:05 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	t_to_die;

	if (get_bool(&philo->mutex, &philo->full))
		return (false);
	elapsed = gettime(MILLISECOND) - get_long(&philo->mutex,
			&philo->last_eaten);
	t_to_die = philo->table->time_to_die / 1e3;
	if (elapsed > t_to_die)
		return (true);
	return (false);
}

void	*monitor_dinner(void *param)
{
	t_table	*table;
	int		i;

	table = (t_table *)param;
	while (!all_thread_running(&table->table_mutex,
			&table->thread_running_number,
			table->philo_nbr))
			;
	while (!simu_finish(table))
	{
		i = -1;
		while (++i < table->philo_nbr && !simu_finish(table))
		{
			if (philo_died(table->philo + i))
			{
				set_bool(&table->table_mutex, &table->end_simu, true);
				write_status(DIED, table->philo + i);
			}
		}
	}
	return (NULL);
}
