/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:04:41 by lpieck            #+#    #+#             */
/*   Updated: 2025/12/05 17:09:06 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	*eat_sleep_think(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!ready_to_go(philo))
		usleep(50);
	if (philo->id % 2 == 0)
		precise_sleep(philo, philo->data->time_to_eat);
	else if (philo->id == philo->data->nb_philo && philo->id % 2 == 1)
		precise_sleep(philo, philo->data->time_to_eat - 5000);
	while (check_if_alive(philo->data))
	{
		// if (!check_if_alive(philo->data))
		// 	break ;
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
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
		return (printf("mem error\n"), free_all_data(data), false);
	memset(data->philos, 0, sizeof(t_philo) * nb_philo);
	return (true);
}
