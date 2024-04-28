/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/28 16:30:12 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	cancel_dinner(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->waiter);
	if ((current_time() - philo->last_meal >= controler()->die_timer)
		|| (philo->meals_ate <= 0 && ((current_time() - philo->start_time
					>= controler()->die_timer)))
		|| controler()->max_philos == 1)
		return (kill_philo(philo));
	if (controler()->all_philos_ate == controler()->max_philos)
	{
		controler()->stop_dinner = 1;
		pthread_mutex_unlock(&controler()->waiter);
		return (1);
	}
	pthread_mutex_unlock(&controler()->waiter);
	return (0);
}

void	*routine(void *arg)
{
	t_philosophers	*philo;

	philo = (t_philosophers *)arg;
	if (philo->id % 2 != 0)
		usleep(1000);
	while (!cancel_dinner(philo) && !stop_dinner())
	{
		if (cancel_dinner(philo))
			break ;
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
		print_logs("%s%lld %d is thinking\n", GREY, philo);
		usleep(1000);
	}
	return (NULL);
}

int	create_threads(t_philosophers *philos)
{
	int	i;

	i = 0;
	while (i < controler()->max_philos)
	{
		if (pthread_create(&philos[i].philo_th, NULL, routine, &philos[i]) != 0)
			return (printf("Error: issue with pthread create.\n"));
		i++;
	}
	i = 0;
	while (i < controler()->max_philos)
	{
		if (pthread_join(philos[i].philo_th, NULL) != 0)
			return (printf("Error: issue with pthread join.\n"));
		i++;
	}
	return (0);
}

void	destroy_mutexes(t_philosophers *philos)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&controler()->printer);
	pthread_mutex_destroy(&controler()->waiter);
	while (i < controler()->max_philos)
	{
		pthread_mutex_destroy(&controler()->forks[i].fork);
		i++;
	}
	free(controler()->forks);
	free(philos);
}

int	main(int argc, char **argv)
{
	t_philosophers	*philos;

	if (parsing(argc, argv) != 0)
		return (1);
	philos = malloc(ft_atoi(argv[1]) * sizeof(t_philosophers));
	if (!philos)
		return (1);
	if (!init_philos(philos, argv))
		return (clean_memory(philos));
	if (create_threads(philos) != 0)
		return (clean_memory(philos));
	destroy_mutexes(philos);
	return (0);
}
