/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:09:56 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/07 17:15:23 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//check for max value for time_to_X?
bool	init_args(int argc, char **argv, t_data *data)
{
	int		i;

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
	int			i;

	data->threads = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->threads)
		return (false);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		if (pthread_create(&data->threads[i], NULL,
				&eat_sleep_think, &data->philos) != 0)
			return (false);
		i++;
	}
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	return (true);
}

bool	init_mutexes(t_data *data)
{
	int				i;
	int				j;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!forks)
		return (false);
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (j < i)
			{
				pthread_mutex_destroy(&data->forks[j]);
				j++;
			}
			free(data->forks);
			return (false);
		}
	}
	return (true);
}
