/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/02 21:22:27 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	cancel_dinner(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->meals);
	if (controler()->all_philos_ate == philo->max_philos)
	{
		pthread_mutex_unlock(&controler()->meals);
		pthread_mutex_lock(&controler()->waiter);
		controler()->stop_dinner = 1;
		pthread_mutex_unlock(&controler()->waiter);
		return (1);
	}
	pthread_mutex_unlock(&controler()->meals);
	pthread_mutex_lock(&controler()->time);
	if ((current_time() - philo->last_meal >= controler()->die_timer))
	{
		pthread_mutex_unlock(&controler()->time);
		return (kill_philo(philo));
	}
	pthread_mutex_unlock(&controler()->time);
	return (0);
}

void	*dead_philo(void *arg)
{
	t_philosophers	*philo;
	int				i;

	philo = (t_philosophers *)arg;
	if (philo->id % 2 != 0)
		usleep(1);
	while (1)
	{
		i = 0;
		while (i < philo->max_philos)
		{
			pthread_mutex_lock(&controler()->philo_on);
			if (!philo->on)
			{
				pthread_mutex_unlock(&controler()->philo_on);
				break ;
			}
			pthread_mutex_unlock(&controler()->philo_on);
			usleep(100);
			if (cancel_dinner(&(philo[i])))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philosophers	*philo;

	philo = (t_philosophers *)arg;
	pthread_mutex_lock(&controler()->philo_on);
	philo->on = true;
	pthread_mutex_unlock(&controler()->philo_on);
	if (took_too_long(philo))
		return (NULL);
	if (philo->id % 2 == 0
		|| (philo->max_philos % 2 != 0 && philo->id == philo->max_philos))
		wait_to_eat((philo->eat_timer * 1000), (philo->sleep_timer * 1000));
	while (!stop_dinner())
	{
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
		print_logs("%lld %d is thinking\n", philo);
		wait_to_eat((philo->eat_timer * 1000), (philo->sleep_timer * 1000));
	}
	return (NULL);
}

int	create_threads(t_philosophers *philos)
{
	int			i;
	pthread_t	th;

	i = 0;
	controler()->start_time = current_time();
	while (i < controler()->max_philos)
	{
		if (pthread_create(&philos[i].philo_th, NULL, routine, &philos[i]) != 0)
			return (printf("Error: issue with pthread create.\n"));
		i++;
	}
	if (pthread_create(&th, NULL, dead_philo, philos) != 0)
		return (printf("Error: issue with pthread create.\n"));
	if (pthread_join(th, NULL) != 0)
		return (printf("Error: issue with pthread join.\n"));
	i = 0;
	while (i < controler()->max_philos)
	{
		if (pthread_join(philos[i].philo_th, NULL) != 0)
			return (printf("Error: issue with pthread join.\n"));
		i++;
	}
	return (0);
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
