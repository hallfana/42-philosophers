/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:59:34 by hallfana          #+#    #+#             */
/*   Updated: 2025/01/09 23:46:05 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <errno.h>

/*
** philosophers project
** usage ./philosophers number_of_philosophers time_to_die time_to_eat
					time_to_sleep [number_of_times_each_philosopher_must_eat]
** number_of_philosophers: number of philosophers and forks
** time_to_die: time in milliseconds before a philosopher dies
** time_to_eat: time in milliseconds for a philosopher to eat
** time_to_sleep: time in milliseconds for a philosopher to sleep
** number_of_times_each_philosopher_must_eat: optional number of times each
														philosopher must eat
*/

typedef enum s_opc
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}	t_opc;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}	t_time_code;

typedef enum e_philo_state
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}	t_philo_state;

typedef struct s_table	t_table;

typedef struct s_fork
{
	pthread_mutex_t	fork;
	int				fork_id;
}	t_fork;

typedef struct s_philo
{
	int				id;
	long			time_eaten;
	long			last_eaten;
	bool			full;
	t_fork			*lfork;
	t_fork			*rfork;
	pthread_t		thread_id;
	t_table			*table;
	pthread_mutex_t	mutex;
}	t_philo;

struct s_table
{
	long			philo_nbr;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			nbr_limit_meals; // populated or -1 for disabled
	long			start_simu;
	long			thread_running_number;
	bool			end_simu;
	bool			simu_ready;
	pthread_t		monitor;
	pthread_mutex_t	table_mutex;
	pthread_mutex_t	write_lock;
	t_fork			*forks;
	t_philo			*philo;
};

void	error_exit(const char *error);
void	parse_argv(t_table *table, char **argv);
void	*safe_malloc(size_t bytes);
void	init(t_table *table);
void	safe_thread_handle(pthread_t *thread, void *(*func)(void *),
			void *arg, t_opc opc);
void	safe_mutex_handle(pthread_mutex_t *mutex, t_opc opc);
bool	simu_finish(t_table *table);
long	get_long(pthread_mutex_t *mutex, long *dest);
void	set_long(pthread_mutex_t *mutex, long *dest, long value);
bool	get_bool(pthread_mutex_t *mutex, bool *dest);
void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value);
void	wait_all_thread(t_table *table);
long	gettime(t_time_code time_code);
void	precise_usleep(long usec, t_table *table);
void	write_status(t_philo_state status, t_philo *philo);
void	dinner(t_table *table);
void	*monitor_dinner(void *param);
void	increase_long(pthread_mutex_t *mutex, long *value);
bool	all_thread_running(pthread_mutex_t *mutex, long *threads,
			long philo_nbr);
void	clean(t_table *table);
void	think(t_philo *philo, bool pre_simu);
void	de_synchronize_philos(t_philo *philo);

#endif