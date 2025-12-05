/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:41:36 by lpieck            #+#    #+#             */
/*   Updated: 2025/12/04 15:08:52 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_status(t_philo *philo, char *msg)
{
	long	start_time;

	start_time = philo->data->start_time;
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead && ft_strncmp(msg, " died", 10) != 0)
	{
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", time_in_ms() - start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	free_all_data(t_data *data)
{
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;
	}
	if (data->threads)
	{
		free(data->threads);
		data->threads = NULL;
	}
	if (data->philos)
	{
		free(data->philos);
		data->threads = NULL;
	}
}

bool	ready_to_go(t_philo *philo)
{
	bool	ready;

	pthread_mutex_lock(&philo->data->ready_mutex);
	ready = philo->data->all_ready;
	pthread_mutex_unlock(&philo->data->ready_mutex);
	return (ready);
}

bool	check_if_alive(t_data *data)
{
	pthread_mutex_lock(&data->dead_mutex);
	if (data->dead)
		return (pthread_mutex_unlock(&data->dead_mutex), false);
	pthread_mutex_unlock(&data->dead_mutex);
	return (true);
}
