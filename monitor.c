/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:37:06 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/14 17:32:56 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	long	elapsed;

	data = arg;
	while (1)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->philos[i].meal_mutex);
			elapsed = time_in_ms() - data->philos[i].last_meal_time;
			pthread_mutex_unlock(&data->philos[i].meal_mutex);
			if (elapsed > data->time_to_die)
			{
				philo_kill(&data->philos[i], data);
				return (NULL);
			}
			i++;
		}
		usleep(250);
	}
}

void	philo_kill(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->dead_mutex);
	if (!philo->dead)
	{
		philo->dead = true;
		print_status(philo, " died");
	}
	pthread_mutex_unlock(&data->dead_mutex);
}
