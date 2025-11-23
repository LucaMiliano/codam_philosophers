/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:04:41 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/14 17:44:47 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2

//TODO: clean up functions

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!init_args(argc, argv, &data))
		return (1);
	memory_allocation(&data);
	init_mutexes(&data);
	init_threads(&data);
	init_monitor(&data);
	join_threads(&data);
	destroy_mutexes(&data);
	free_all_data(&data);
	return (0);
}

void	*eat_sleep_think(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->last_meal_time = time_in_ms();
	while (1)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead)
			break;
		pthread_mutex_unlock(&philo->data->dead_mutex);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

void	philo_eat(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->data->nb_philo;
	if (philo->id % 2 != 0)
	{
		pthread_mutex_lock(&philo->data->forks[right]);
		print_status(philo, " has taken a fork");
		pthread_mutex_lock(&philo->data->forks[left]);
		print_status(philo, " has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[left]);
		print_status(philo, " has taken a fork");
		pthread_mutex_lock(&philo->data->forks[right]);
		print_status(philo, " has taken a fork");
	}
	print_status(philo, " is eating");
	philo->meals_eaten++;
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->data->forks[left]);
	pthread_mutex_unlock(&philo->data->forks[right]);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, " is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	print_status(philo, " is thinking");
}
