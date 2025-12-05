/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 14:17:17 by lpieck            #+#    #+#             */
/*   Updated: 2025/12/05 17:00:09 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_eat(t_philo *philo)
{
	int	left;
	int	right;

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
	print_status(philo, " is sleeping");
	precise_sleep(philo, philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, " is thinking");
	if (philo->data->nb_philo % 2 != 0)
	{
		// if (philo->id % 2 != 0)
		// 	usleep(500);
		// else if (philo->id == philo->data->nb_philo)
		// 	usleep(750);
		think_time = (philo->data->time_to_eat * 2 - philo->data->time_to_sleep);
        if (think_time < 0)
            think_time = 0;
        think_time = think_time * 1000 / philo->data->nb_philo;
        if (think_time > 0)
            usleep(think_time);
	}
}

void	pick_up_forks(t_philo *philo, int left, int right)
{
	if (philo->data->nb_philo > 1 && philo->id == philo->data->nb_philo)
	{
		// usleep(500);
		pthread_mutex_lock(&philo->data->forks[left]);
		print_status(philo, " has taken a fork");
		pthread_mutex_lock(&philo->data->forks[right]);
		print_status(philo, " has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[right]);
		print_status(philo, " has taken a fork");
		while (philo->data->nb_philo == 1)
		{
			// usleep(500);
			if (!check_if_alive(philo->data))
			{
				pthread_mutex_unlock(&philo->data->forks[right]);
				return ;
			}
		}
		pthread_mutex_lock(&philo->data->forks[left]);
		print_status(philo, " has taken a fork");
	}
}

void	precise_sleep(t_philo *philo, long duration_ms)
{
	long	start;
	long	elapsed;

	start = time_in_ms();
	while (1)
	{
		elapsed = time_in_ms() - start;
		if (elapsed >= duration_ms)
			break ;
		if (!check_if_alive(philo->data))
			break ;
		if (duration_ms - elapsed > 20)
			usleep(10000);
		else if (duration_ms - elapsed > 5)
			usleep(2000);
		else
			usleep(200);
	}
}
