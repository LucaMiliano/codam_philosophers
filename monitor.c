/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:37:06 by lpieck            #+#    #+#             */
/*   Updated: 2025/12/23 12:37:22 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_kill(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->dead_mutex);
	if (!data->dead)
	{
		data->dead = true;
		pthread_mutex_unlock(&data->dead_mutex);
		print_status(philo, "died");
		return ;
	}
	pthread_mutex_unlock(&data->dead_mutex);
}

void	wait_for_ready(t_data *data)
{
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
	return ;
}

bool	check_to_kill(t_data *data, int i)
{
	long	elapsed;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	elapsed = time_in_ms() - data->philos[i].last_meal_time;
	if (elapsed > data->time_to_die)
	{
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		philo_kill(&data->philos[i], data);
		return (true);
	}
	return (false);
}

int	go_through_philos(t_data *data)
{
	int	i;
	int	all_eaten;

	i = 0;
	all_eaten = 0;
	while (i < data->nb_philo)
	{
		if (check_to_kill(data, i))
			return (0);
		if (data->must_eat > 0 && data->philos[i].meals_eaten >= data->must_eat)
			all_eaten++;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	return (all_eaten);
}

void	*monitor(void *arg)
{
	t_data		*data;
	int			all_eaten;

	data = arg;
	wait_for_ready(data);
	while (1)
	{
		if (!check_if_alive(data))
			return (NULL);
		all_eaten = go_through_philos(data);
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
