/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpieck <lpieck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:05:42 by lpieck            #+#    #+#             */
/*   Updated: 2025/11/28 15:52:13 by lpieck           ###   ########.fr       */
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
	bool			dead;
	bool			all_ready;
	long			start_time;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				forks_mx_count;
	pthread_t		*threads;
	pthread_mutex_t	print_mutex;
	bool			print_mx_init;
	pthread_mutex_t	dead_mutex;
	bool			dead_mx_init;
	pthread_mutex_t	ready_mutex;
	bool			ready_mx_init;
	pthread_t		monitor;
	int				meal_mx_count;
}	t_data;

struct s_philo
{
	int				id;
	long			last_meal_time;
	pthread_mutex_t	meal_mutex;
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
bool	memory_allocation(t_data *data, int nb_philo);
void	join_threads(t_data *data);
void	print_status(t_philo *philo, char *msg);
bool	check_if_alive(t_data *data);
void	pick_up_forks(t_philo *philo, int left, int right);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	precise_sleep(t_philo *philo, long duration_ms);

#endif
