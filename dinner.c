/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:18:19 by samberna          #+#    #+#             */
/*   Updated: 2025/01/09 23:47:18 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	think(t_philo *philo, bool pre_simu)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!pre_simu)
		write_status(THINKING, philo);
	if (philo->table->philo_nbr % 2 == 0)
		return ;
	t_eat = philo->table->time_to_eat;
	t_sleep = philo->table->time_to_sleep;
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0;
	precise_usleep(t_think * 0.5, philo->table);
}

static void	eat(t_philo *philo)
{
	safe_mutex_handle(&philo->lfork->fork, LOCK);
	write_status(TAKE_FIRST_FORK, philo);
	safe_mutex_handle(&philo->rfork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo);
	set_long(&philo->mutex, &philo->last_eaten, gettime(MILLISECOND));
	philo->time_eaten++;
	write_status(EATING, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->nbr_limit_meals > 0
		&& philo->time_eaten == philo->table->nbr_limit_meals)
		set_bool(&philo->mutex, &philo->full, true);
	safe_mutex_handle(&philo->lfork->fork, UNLOCK);
	safe_mutex_handle(&philo->rfork->fork, UNLOCK);
}

void	*lone_philo(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	wait_all_thread(philo->table);
	set_long(&philo->mutex, &philo->last_eaten, gettime(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->thread_running_number);
	write_status(TAKE_FIRST_FORK, philo);
	while (!simu_finish(philo->table))
		usleep(200);
	return (NULL);
}

void	*dinner_thread(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	wait_all_thread(philo->table);
	set_long(&philo->mutex, &philo->last_eaten, gettime(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->thread_running_number);
	de_synchronize_philos(philo);
	while (!simu_finish(philo->table))
	{
		if (philo->full)
			break ;
		eat(philo);
		write_status(SLEEPING, philo);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		think(philo, false);
	}
	return (NULL);
}

void	dinner(t_table *table)
{
	int	i;

	i = -1;
	if (table->nbr_limit_meals == 0)
		return ;
	else if (table->philo_nbr == 1)
		safe_thread_handle(&table->philo[0].thread_id, lone_philo,
			&table->philo[0], CREATE);
	else
	{
		while (++i < table->philo_nbr)
			safe_thread_handle(&table->philo[i].thread_id, dinner_thread,
				&table->philo[i], CREATE);
	}
	safe_thread_handle(&table->monitor, monitor_dinner, table, CREATE);
	table->start_simu = gettime(MILLISECOND);
	set_bool(&table->table_mutex, &table->simu_ready, true);
	i = -1;
	while (++i < table->philo_nbr)
		safe_thread_handle(&table->philo[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simu, true);
	safe_thread_handle(&table->monitor, NULL, NULL, JOIN);
}
