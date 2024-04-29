/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dinner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:21:35 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/29 14:28:48 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	init_controler(char **argv)
{
	t_fork		*fork;

	controler()->max_philos = ft_atoi(argv[1]);
	controler()->all_philos_ate = 0;
	controler()->stop_dinner = 0;
	fork = malloc(controler()->max_philos * sizeof(t_fork));
	if (!fork)
		return (0);
	init_forks(fork);
	controler()->forks = fork;
	pthread_mutex_init(&controler()->waiter, NULL);
	pthread_mutex_init(&controler()->printer, NULL);
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

int	init_philos(t_philosophers *philo, char **argv)
{
	int	i;

	if (!init_controler(argv))
		return (0);
	i = 0;
	while (i < controler()->max_philos)
	{
		philo[i].max_philos = ft_atoi(argv[1]);
		philo[i].die_timer = ft_atoi(argv[2]);
		philo[i].eat_timer = ft_atoi(argv[3]);
		philo[i].sleep_timer = ft_atoi(argv[4]);
		if (argv[5])
			philo[i].max_meals = ft_atoi(argv[5]);
		else
			philo[i].max_meals = -1;
		philo[i].id = i + 1;
		philo[i].start_time = current_time();
		philo[i].last_meal = current_time();
		philo[i].meals_ate = 0;
		philo[i].rfork = i;
		if (philo[i].id == controler()->max_philos)
		{
			philo[i].lfork = i;
			philo[i].rfork = 0;
		}
		else
			philo[i].lfork = i + 1;
		i++;
	}
	return (1);
}
