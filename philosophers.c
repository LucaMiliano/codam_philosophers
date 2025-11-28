/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:04:41 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/28 17:11:30 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO: clean up functions

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!init_args(argc, argv, &data))
		return (1);
	init_mutexes(&data);
	init_threads(&data);
	init_monitor(&data);
	join_threads(&data);
	destroy_mutexes(&data);
	free_all_data(&data);
	return (0);
}

bool	ready_to_go(t_philo *philo)
{
	bool	ready;

	pthread_mutex_lock(&philo->data->ready_mutex);
	ready = philo->data->all_ready;
	pthread_mutex_unlock(&philo->data->ready_mutex);
	return (ready);
}

void	*eat_sleep_think(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!ready_to_go(philo))
		usleep(100);
	if (philo->id % 2 == 0)
		usleep(500);
	while (1)
	{
		if (!check_if_alive(philo->data))
			break ;
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

void	philo_eat(t_philo *philo)
{
	int	i;
	int	left;
	int	right;

	i = 0;
	left = philo->id % philo->data->nb_philo;
	right = philo->id - 1;
	pick_up_forks(philo, left, right);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = time_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status(philo, " is eating");
	precise_sleep(philo, philo->data->time_to_eat);
	if (philo->data->nb_philo != 1)
	{
		pthread_mutex_unlock(&philo->data->forks[left]);
		pthread_mutex_unlock(&philo->data->forks[right]);
	}
}

void	philo_sleep(t_philo *philo)
{
	int	i;

	i = 0;
	print_status(philo, " is sleeping");
	precise_sleep(philo, philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_status(philo, " is thinking");
	if (philo->id % 2 == 0)
		usleep(1500);
}

void	pick_up_forks(t_philo *philo, int left, int right)
{
	if (left > right)
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
		while (philo->data->nb_philo == 1)
		{
			usleep(50);
			if (!check_if_alive(philo->data))
			{
				pthread_mutex_unlock(&philo->data->forks[left]);
				return ;
			}
		}
		pthread_mutex_lock(&philo->data->forks[right]);
		print_status(philo, " has taken a fork");
	}
}
