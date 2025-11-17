/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:09:56 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/14 17:50:50 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//check for max value for time_to_X?
bool	init_args(int argc, char **argv, t_data *data)
{
	int	i;

	i = 1;
	if (argc < 5 || argc > 6)
		return (printf("Unable to run, wrong amount of args.\n"), false);
	while (argv)
	{
		if (!ft_isdigit(argv[i]))
			return (printf("Only (positive) nums are valid inputs.\n"), false);
		if (ft_atoi_safe(argv[i]) > INT_MAX)
			return (printf("Keep input under INT_MAX.\n"), false);
		i++;
	}
	data->nb_philo = ft_atoi_safe(argv[1]);
	data->time_to_die = ft_atoi_safe(argv[2]);
	data->time_to_eat = ft_atoi_safe(argv[3]);
	data->time_to_sleep = ft_atoi_safe(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi_safe(argv[5]);
	return (true);
}

bool	init_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i;
		data->philos[i].data = data;
		if (pthread_create(&data->threads[i], NULL,
				eat_sleep_think, &data->philos) != 0)
			return (false);
		i++;
	}
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
	}
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
	}
	return (true);
}

bool	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&data->dead_mutex), false);
	if (!init_forks(data))
	{
		pthread_mutex_destroy(&data->dead_mutex);
		pthread_mutex_destroy(&data->print_mutex);
		return (false);
	}
	if (!init_meals(data))
	{
		pthread_mutex_destroy(&data->dead_mutex);
		pthread_mutex_destroy(&data->print_mutex);
		while (i < data->nb_philo)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		return (false);
	}
	return (true);
}


bool	init_monitor(t_data *data)
{
	if ((pthread_create(&data->monitor, NULL, monitor(data), &data)) != 0)
		return (false);
	return (true);
}

bool	memory_allocation(t_data *data)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	data->threads = malloc(sizeof(pthread_t) * data->nb_philo);
	data->philos = malloc (sizeof(t_philo) * data->nb_philo);
	if (!data->forks || !data->threads || data->philos)
		return (free_all_data(data), false);
	return (true);
}


