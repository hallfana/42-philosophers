/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controlled_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:50:30 by samberna          #+#    #+#             */
/*   Updated: 2025/01/09 23:41:57 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		error_exit("error during a call to malloc\n");
	return (ret);
}

static void	mutex_error(int status, t_opc opc)
{
	if (status == 0)
		return ;
	else if (EINVAL == status && (opc == LOCK || opc == UNLOCK
			|| opc == DESTROY))
		error_exit("the value specified by mutex is invalid\n");
	else if (EINVAL == status && (opc == INIT))
		error_exit("the value specified by attr is invalid\n");
	else if (EDEADLK == status)
		error_exit("A dead lock would occur"
			" if the thread blocked waiting for mutex\n");
	else if (EPERM == status)
		error_exit("the current thread does not hold a lock on mutex\n");
	else if (ENOMEM == status)
		error_exit("The process cannot allocate enough memory to create"
			" another mutex\n");
	else if (EBUSY == status)
		error_exit("Mutex is locked\n");
}

void	safe_mutex_handle(pthread_mutex_t *mutex, t_opc opc)
{
	if (opc == LOCK)
		mutex_error(pthread_mutex_lock(mutex), opc);
	else if (opc == UNLOCK)
		mutex_error(pthread_mutex_unlock(mutex), opc);
	else if (opc == INIT)
		mutex_error(pthread_mutex_init(mutex, NULL), opc);
	else if (opc == DESTROY)
		mutex_error(pthread_mutex_destroy(mutex), opc);
	else
		error_exit("wrong opc for the mutex handle"
			" use <LOCK UNLOCK INIT DESTROY>\n");
}

static void	thread_error(int status, t_opc opc)
{
	if (status == 0)
		return ;
	if (EAGAIN == status)
		error_exit("Insufficient resources to create another thread\n");
	else if (EPERM == status)
		error_exit("The caller does not have appropriate perimission\n");
	else if (EINVAL == status && CREATE == opc)
		error_exit("The value specified by attr is invalid\n");
	else if (EINVAL == status && (JOIN == opc || DETACH == opc))
		error_exit("The thread is not joinable\n");
	else if (ESRCH == status)
		error_exit("No thread could be found corresponding to that specified"
			" by the given thread ID, thread\n");
	else if (EDEADLK == status)
		error_exit("A deadlock was detected or the value of thread specifies"
			" the calling thread\n");
}

void	safe_thread_handle(pthread_t *thread, void *(*func)(void *),
	void *arg, t_opc opc)
{
	if (opc == CREATE)
		thread_error(pthread_create(thread, NULL, func, arg), opc);
	else if (opc == JOIN)
		thread_error(pthread_join(*thread, NULL), opc);
	else if (opc == DETACH)
		thread_error(pthread_detach(*thread), opc);
	else
		error_exit("wrong opc for the thread handle"
			" use <CREATE JOIN DETACH>\n");
}
