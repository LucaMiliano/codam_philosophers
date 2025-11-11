/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:04:41 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/07 17:18:12 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2

//TODO: clean up functions

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!init_args(argc, argv, &data) || !init_threads(&data))
		return (1);
	if (!init_mutexes(&data))
		return (1);
	return (0);
}

void	*eat_sleep_think(void *arg)
{
	t_philo	*philo;	

	philo = (t_philo *)arg;
	while (!philo->dead && philo->meals_eaten < philo->data->must_eat)
	{
		philo_think(philo);
		philo_eat(philo);
		philo_sleep(philo);
	}
	if (philo->dead)
	{
		printf("%d %d died\n", time_in_ms(), philo->id);
		clean_exit();
	}
	else if (philo->meals_eaten >= philo->data->must_eat)
		clean_exit();
	eat_sleep_think(philo);	
}

long	philo_eat(t_philo *philo)
{
	philo->fork_right = data->forks[philo->id - 1];
	philo->fork_left = data->forks[philo->id];
	if (philo->id == philo->data->nb_philo)
		philo->fork_left = data->forks[0];
	if (philo->id % 2 != 0)
	{
	pthread_mutex_lock(philo->fork_right);
	printf("%d %d has taken a fork\n", time_in_ms(), philo->id);
	pthread_mutex_lock(philo->fork_left);
	printf("%d %d has taken a fork\n", time_in_ms(), philo->id);
	}
	else
	{
	pthread_mutex_lock(philo->fork_left);
	printf("%d %d has taken a fork\n", time_in_ms(), philo->id);
	pthread_mutex_lock(philo->fork_right);
	printf("%d %d has taken a fork\n", time_in_ms(), philo->id);
	}
	printf("%d %d is eating\n", time_in_ms(), philo->id);
	philo->meals_eaten++;
	usleep(philo->data->time_to_eat * 1000);
}

long	philo_sleep(t_philo *philo)
{
	printf("%d %d is sleeping\n", time_in_ms(), philo->id);
	usleep(philo->data->time_to_sleep  * 1000);
}

long	philo_think(t_philo *philo)
{
	printf("%d %d is thinking\n", time_in_ms(), philo->id);
}