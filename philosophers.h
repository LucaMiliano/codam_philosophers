/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:05:42 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/14 17:51:51 by lpieck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dead_mutex;
	pthread_t		monitor;
}	t_data;

struct s_philo
{
	int				id;
	long			last_meal_time;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	fork_left;
	pthread_mutex_t	fork_right;
	bool			dead;
	int				meals_eaten;
	t_data			*data;
}	;

bool	init_args(int argc, char **argv, t_data *data);
bool	init_threads(t_data *data);
bool	init_mutexes(t_data *data);
bool	init_monitor(t_data *data);

long	ft_atoi_safe(const char *nptr);
int		ft_isdigit(char *str);
void	*eat_sleep_think(void *arg);
long	time_in_ms(void);
void	philo_kill(t_philo *philo, t_data *data);
void	*monitor(void *arg);
void	free_all_data(t_data *data);
void	destroy_mutexes(t_data *data);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
bool	memory_allocation(t_data *data);
void	join_threads(t_data *data);
void	print_status(t_philo *philo, char *msg);

#endif
