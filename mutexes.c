/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:25:13 by lpieck            #+#    #+#             */
/*   Updated: 2025/12/18 16:54:10 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	init_meals(t_data *data)
{
	int				i;
	int				j;

	i = 0;
	j = 0;
	data->meal_mx_count = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
		{
			while (j < i)
			{
				pthread_mutex_destroy(&data->philos[j].meal_mutex);
				j++;
			}
			return (false);
		}
		i++;
		data->meal_mx_count++;
	}
	return (true);
}

bool	init_forks(t_data *data)
{
	int				i;
	int				j;

	i = 0;
	j = 0;
	data->forks_mx_count = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (j < i)
			{
				pthread_mutex_destroy(&data->forks[j]);
				j++;
			}
			return (false);
		}
		i++;
		data->forks_mx_count++;
	}
	return (true);
}

void	destroy_mutexes(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (data->dead_mx_init == true)
		pthread_mutex_destroy(&data->dead_mutex);
	if (data->print_mx_init == true)
		pthread_mutex_destroy(&data->print_mutex);
	if (data->ready_mx_init == true)
		pthread_mutex_destroy(&data->ready_mutex);
	while (i < data->forks_mx_count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	while (j < data->meal_mx_count)
	{
		pthread_mutex_destroy(&data->philos[j].meal_mutex);
		j++;
	}
}

bool	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->ready_mutex, NULL) != 0)
		return (destroy_mutexes(data), false);
	data->ready_mx_init = true;
	if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
		return (destroy_mutexes(data), false);
	data->dead_mx_init = true;
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (destroy_mutexes(data), false);
	data->print_mx_init = true;
	if (!init_forks(data))
		return (destroy_mutexes(data), false);
	if (!init_meals(data))
		return (destroy_mutexes(data), false);
	return (true);
}
