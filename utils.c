/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:41:36 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/28 15:51:59 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	ft_atoi_safe(const char *nptr)
{
	int		i;
	long	result;
	int		sign;

	i = 0;
	result = 0;
	sign = 1;
	while (nptr[i] == '\f' || nptr[i] == '\n' || nptr[i] == '\r'
		|| nptr[i] == '\t' || nptr[i] == '\v' || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i])
	{
		result = result * 10 + (nptr[i] - '0');
		if (result > INT_MAX)
			return (result * sign);
		i++;
	}
	return (result * sign);
}

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long	time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

//change to ft_strcmp
void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead && ft_strncmp(msg, " died", 10) != 0)
	{
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", time_in_ms() - philo->data->start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	free_all_data(t_data *data)
{
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;
	}
	if (data->threads)
	{
		free(data->threads);
		data->threads = NULL;
	}
	if (data->philos)
	{
		free(data->philos);
		data->threads = NULL;
	}
}

bool	check_if_alive(t_data *data)
{
	pthread_mutex_lock(&data->dead_mutex);
	if (data->dead)
		return (pthread_mutex_unlock(&data->dead_mutex), false);
	pthread_mutex_unlock(&data->dead_mutex);
	return (true);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
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
		if (duration_ms - elapsed > 10)
			usleep(5000);
		else
			usleep(500);
	}
}
