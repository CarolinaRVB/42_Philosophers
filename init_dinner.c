/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dinner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:21:35 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/02 17:52:30 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_controler(char **argv)
{
	controler()->max_philos = ft_atoi(argv[1]);
	controler()->die_timer = ft_atoi(argv[2]);
	controler()->all_philos_ate = 0;
	controler()->stop_dinner = 0;
	controler()->forks = malloc(controler()->max_philos * sizeof(t_fork));
	if (!controler()->forks)
		return (0);
	init_forks(controler()->forks);
	if (pthread_mutex_init(&controler()->waiter, NULL)
		|| pthread_mutex_init(&controler()->printer, NULL)
		|| pthread_mutex_init(&controler()->time, NULL)
		|| pthread_mutex_init(&controler()->meals, NULL)
		|| pthread_mutex_init(&controler()->philo_on, NULL))
		return (0);
	return (1);
}

void	init_forks(t_fork *fork)
{
	int	i;

	i = 0;
	while (i < controler()->max_philos)
	{
		pthread_mutex_init(&fork[i].fork, NULL);
		i++;
	}
}

void	set_philo_meal_fork(t_philosophers *philo, char **argv, int i)
{
	if (argv[5])
		philo[i].max_meals = ft_atoi(argv[5]);
	else
		philo[i].max_meals = -1;
	if (philo[i].id == philo[i].max_philos)
	{
		philo[i].lfork = i;
		philo[i].rfork = 0;
	}
	else
		philo[i].lfork = i + 1;
}

int	init_philos(t_philosophers *philo, char **argv)
{
	int	i;

	if (!init_controler(argv))
		return (0);
	i = 0;
	while (i < controler()->max_philos)
	{
		philo[i].id = i + 1;
		philo[i].eat_timer = ft_atoi(argv[3]);
		philo[i].sleep_timer = ft_atoi(argv[4]);
		philo[i].max_philos = ft_atoi(argv[1]);
		philo[i].meals_ate = 0;
		philo[i].rfork = i;
		philo[i].on = false;
		set_philo_meal_fork(philo, argv, i);
		philo[i].start_time = current_time();
		philo[i].last_meal = current_time();
		i++;
	}
	return (1);
}

void	destroy_mutexes(t_philosophers *philos)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&controler()->printer);
	pthread_mutex_destroy(&controler()->waiter);
	pthread_mutex_destroy(&controler()->time);
	pthread_mutex_destroy(&controler()->meals);
	pthread_mutex_destroy(&controler()->philo_on);
	while (i < controler()->max_philos)
	{
		pthread_mutex_destroy(&controler()->forks[i].fork);
		i++;
	}
	free(controler()->forks);
	free(philos);
}
