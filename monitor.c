/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:37:06 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/25 14:07:02 by lpieck           ###   ########.fr       */
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
		if (data->all_ready == true)
		{
			pthread_mutex_unlock(&data->ready_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->ready_mutex);
		usleep(1000);
	}
	while (1)
	{
		i = 0;
		all_eaten = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->philos[i].meal_mutex);
			elapsed = time_in_ms() - data->philos[i].last_meal_time;
			if (data->must_eat > 0 && data->philos[i].meals_eaten >= data->must_eat)
				all_eaten++;
			if (elapsed > data->time_to_die)
				return (pthread_mutex_unlock(&data->philos[i].meal_mutex), philo_kill(&data->philos[i], data), NULL);
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
		usleep(100);
	}
	return (NULL);
}

void	philo_kill(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->dead_mutex);
	if (!data->dead)
	{
		print_status(philo, " died");
		data->dead = true;
	}
	pthread_mutex_unlock(&data->dead_mutex);
}
