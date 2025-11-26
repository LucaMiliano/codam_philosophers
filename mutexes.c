/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:25:13 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/14 17:27:37 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//needs to be protected against undefined behaviour
void	destroy_mutexes(t_data *data)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	if (data->dead_mx_init == true)
		pthread_mutex_destroy(&data->dead_mutex);
	if (data->print_mx_init == true)
		pthread_mutex_destroy(&data->print_mutex);
	if (data->ready_mx_init == true)
		pthread_mutex_destroy(&data->ready_mutex);
	while (i < data->forks_mx_count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	while (j < data->meal_mx_count)
	{
		pthread_mutex_destroy(&data->philos[j].meal_mutex);
		j++;
	}
}
