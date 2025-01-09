/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:48:14 by samberna          #+#    #+#             */
/*   Updated: 2025/01/09 23:49:35 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int p)
{
	int	philo_nbr;

	philo_nbr = philo->table->philo_nbr;
	if (philo->id % 2 == 0)
	{
		philo->lfork = &forks[p];
		philo->rfork = &forks[(p + 1) % philo_nbr];
	}
	else
	{
		philo->rfork = &forks[p];
		philo->lfork = &forks[(p + 1) % philo_nbr];
	}
}

static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philo + i;
		philo->id = i + 1;
		philo->full = false;
		philo->time_eaten = 0;
		philo->table = table;
		assign_forks(philo, table->forks, i);
		safe_mutex_handle(&philo->mutex, INIT);
	}
}

void	init(t_table *table)
{
	int	i;

	i = -1;
	table->end_simu = false;
	table->simu_ready = false;
	table->thread_running_number = 0;
	table->philo = safe_malloc(sizeof(t_philo) * table->philo_nbr);
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
	safe_mutex_handle(&table->table_mutex, INIT);
	safe_mutex_handle(&table->write_lock, INIT);
	while (++i < table->philo_nbr)
	{
		safe_mutex_handle(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i;
	}
	philo_init(table);
}
