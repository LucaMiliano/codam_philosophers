/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:09:56 by lpieck            #+#    #+#             */
/*   Updated: 2025/12/18 16:40:41 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>

bool	init_threads(t_data *data)
{
	int	i;

	i = 0;
	data->all_ready = false;
	data->start_time = time_in_ms();
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = time_in_ms();
		if (pthread_create(&data->threads[i], NULL,
				eat_sleep_think, &data->philos[i]) != 0)
			break ;
		i++;
	}
	if (i < data->nb_philo)
	{
		end_threads(data, i);
		return (false);
	}
	set_ready(data);
	return (true);
}

void	set_ready(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->ready_mutex);
	data->start_time = time_in_ms();
	while (i < data->nb_philo)
	{
		data->philos[i].last_meal_time = data->start_time;
		i++;
	}
	data->all_ready = true;
	pthread_mutex_unlock(&data->ready_mutex);
}

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	if (data->monitor_set == true)
		pthread_join(data->monitor, NULL);
}

bool	init_monitor(t_data *data)
{
	if ((pthread_create(&data->monitor, NULL, monitor, data)) != 0)
	{
		data->dead = true;
		return (false);
	}
	data->monitor_set = true;
	return (true);
}

void	end_threads(t_data *data, int i)
{
	data->dead = true;
	while (i >= 0)
	{
		pthread_join(data->threads[i], NULL);
		i--;
	}
}
