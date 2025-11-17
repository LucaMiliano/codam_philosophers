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
	return (0);
}

void	*eat_sleep_think(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->last_meal_time = time_in_ms();
	philo->dead = false;
	philo->fork_right = philo->data->forks[philo->id - 1];
	philo->fork_left = philo->data->forks[philo->id];
	if (philo->id == philo->data->nb_philo)
		philo->fork_left = philo->data->forks[0];
	while (!philo->dead)
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

void	philo_eat(t_philo *philo)
{
	if (philo->id % 2 != 0)
	{
		pthread_mutex_lock(&philo->fork_right);
		printf("%ld %d has taken a fork\n", time_in_ms(), philo->id);
		pthread_mutex_lock(&philo->fork_left);
		printf("%ld %d has taken a fork\n", time_in_ms(), philo->id);
	}
	else
	{
		pthread_mutex_lock(&philo->fork_left);
		printf("%ld %d has taken a fork\n", time_in_ms(), philo->id);
		pthread_mutex_lock(&philo->fork_right);
		printf("%ld %d has taken a fork\n", time_in_ms(), philo->id);
	}
	printf("%ld %d is eating\n", time_in_ms(), philo->id);
	philo->meals_eaten++;
	usleep(philo->data->time_to_eat * 1000);
}

void	philo_sleep(t_philo *philo)
{
	printf("%ld %d is sleeping\n", time_in_ms(), philo->id);
	usleep(philo->data->time_to_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	printf("%ld %d is thinking\n", time_in_ms(), philo->id);
}
