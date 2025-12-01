/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:37:06 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/28 14:44:51 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitor(void *arg)
{
	t_data		*data;
	int			i;
	long		elapsed;
	int			all_eaten;

	data = arg;
	all_eaten = 0;
	while (1)
	{
		pthread_mutex_lock(&data->ready_mutex);
		if (data->all_ready)
		{
			pthread_mutex_unlock(&data->ready_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->ready_mutex);
		usleep(100);
	}
	while (1)
	{
		if (!check_if_alive(data))
			return (NULL);
		i = 0;
		all_eaten = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->philos[i].meal_mutex);
			elapsed = time_in_ms() - data->philos[i].last_meal_time;
			if (elapsed > data->time_to_die)
			{
				pthread_mutex_unlock(&data->philos[i].meal_mutex);
				philo_kill(&data->philos[i], data);
				return (NULL);
			}
			if (data->must_eat > 0 && data->philos[i].meals_eaten >= data->must_eat)
				all_eaten++;
			pthread_mutex_unlock(&data->philos[i].meal_mutex);
			i++;
		}
		if (data->must_eat > 0 && all_eaten == data->nb_philo)
		{
			pthread_mutex_lock(&data->dead_mutex);
			data->dead = true;
			pthread_mutex_unlock(&data->dead_mutex);
			return (NULL);
		}
		usleep(50);
	}
	return (NULL);
}

void	philo_kill(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->dead_mutex);
	if (!data->dead)
	{
		data->dead = true;
		pthread_mutex_unlock(&data->dead_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%ld %d died\n", time_in_ms() - data->start_time, philo->id);
		pthread_mutex_unlock(&data->print_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->dead_mutex);
}
