/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:32:06 by samberna          #+#    #+#             */
/*   Updated: 2025/01/09 23:45:02 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

bool	get_bool(pthread_mutex_t *mutex, bool *dest)
{
	bool	ret;

	safe_mutex_handle(mutex, LOCK);
	ret = *dest;
	safe_mutex_handle(mutex, UNLOCK);
	return (ret);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

long	get_long(pthread_mutex_t *mutex, long *dest)
{
	long	ret;

	safe_mutex_handle(mutex, LOCK);
	ret = *dest;
	safe_mutex_handle(mutex, UNLOCK);
	return (ret);
}

bool	simu_finish(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->end_simu));
}
