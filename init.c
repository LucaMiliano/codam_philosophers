/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:09:56 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/25 15:01:28 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>

//check for max value for time_to_X?
bool	init_args(int argc, char **argv, t_data *data)
{
	int	i;

	i = 1;
	if (argc < 5 || argc > 6)
		return (printf("Unable to run, wrong amount of args.\n"), false);
	while (i < argc)
	{
		if (!ft_isdigit(argv[i]))
			return (printf("Only (positive) nums are valid inputs.\n"), false);
		if (ft_atoi_safe(argv[i]) > INT_MAX)
			return (printf("Keep input under INT_MAX.\n"), false);
		i++;
	}
	data->nb_philo = ft_atoi_safe(argv[1]);
	if (data->nb_philo < 1 || data->nb_philo > 200)
		return (false);
	memory_allocation(data, data->nb_philo);
	data->time_to_die = ft_atoi_safe(argv[2]);
	data->time_to_eat = ft_atoi_safe(argv[3]);
	data->time_to_sleep = ft_atoi_safe(argv[4]);
	data->must_eat = -1;
	if (argc == 6)
		data->must_eat = ft_atoi_safe(argv[5]);
	return (true);
}

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
			return (false);
		i++;
	}
	pthread_mutex_lock(&data->ready_mutex);
	// data->start_time = time_in_ms();
	data->all_ready = true;
	pthread_mutex_unlock(&data->ready_mutex);
	return (true);
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
	pthread_join(data->monitor, NULL);
}

bool	init_forks(t_data *data)
{
	int				i;
	int				j;

	i = 0;
	j = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (j < i)
			{
				pthread_mutex_destroy(&data->forks[j]);
				j++;
			}
			free_all_data(data);
			return (false);
		}
		i++;
		data->forks_mx_count++;
	}
	printf("forks init\n");
	return (true);
}

bool	init_meals(t_data *data)
{
	int				i;
	int				j;

	i = 0;
	j = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
		{
			while (j < i)
			{
				pthread_mutex_destroy(&data->philos[j].meal_mutex);
				j++;
			}
			free_all_data(data);
			return (false);
		}
		i++;
		data->meal_mx_count++;
	}
	printf("meals init\n");
	return (true);
}

//rewrite this cleanly? make sure ready mutex gets cleaned if smth goes wrong
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
	printf("mutexes init\n");
	return (true);
}


bool	init_monitor(t_data *data)
{
	if ((pthread_create(&data->monitor, NULL, monitor, data)) != 0)
		return (false);
	// printf("monitor init\n");
	return (true);
}

bool	memory_allocation(t_data *data, int philo_count)
{
	int	nb_philo;

	nb_philo = philo_count;
	memset(data, 0, sizeof(t_data));
	data->nb_philo = nb_philo;
	data->forks = malloc(sizeof(pthread_mutex_t) * nb_philo);
	data->threads = malloc(sizeof(pthread_t) * nb_philo);
	data->philos = malloc (sizeof(t_philo) * nb_philo);
	if (!data->forks || !data->threads || !data->philos)
		return (printf("mem fail\n"), free_all_data(data), false);
	memset(data->philos, 0, sizeof(t_philo) * nb_philo);
	printf("Num of philos: %d.\n", data->nb_philo);
	return (true);
}


