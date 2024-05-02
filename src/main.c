/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/01 19:59:33 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	cancel_dinner(t_philosophers *philo)
{
	if (pthread_mutex_lock(&controler()->waiter) != 0)
		return (1);
	pthread_mutex_lock(&controler()->meals_lock);
	if (controler()->all_philos_ate == controler()->max_philos)
	{
		controler()->stop_dinner = 1;
		pthread_mutex_unlock(&controler()->meals_lock);
		pthread_mutex_unlock(&controler()->waiter);
		return (1);
	}
	pthread_mutex_unlock(&controler()->meals_lock);
	pthread_mutex_lock(&controler()->timer);
	if ((current_time() - philo->last_meal >= controler()->die_timer))
	{
		
		pthread_mutex_unlock(&controler()->timer);
		pthread_mutex_unlock(&controler()->waiter);
		return (kill_philo(philo));
	}
	pthread_mutex_unlock(&controler()->timer);
	pthread_mutex_unlock(&controler()->waiter);
	return (0);
}

void	*dead_philo(void *arg)
{
	t_philosophers	*philo;
	int				i;

	i = 0;
	// pthread_mutex_lock(&controler()->timer);
	// pthread_mutex_unlock(&controler()->timer);
	philo = (t_philosophers *)arg;
	while (1)
	{
		i = 0;
		while (i < controler()->max_philos)
		{
			if (!philo[i].on)
				break ;
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

	pthread_mutex_lock(&controler()->timer);
	if (!controler()->start_time)
		controler()->start_time = current_time();
	pthread_mutex_unlock(&controler()->timer);
	
	philo = (t_philosophers *)arg;
	philo->on = true;
	if (philo->id % 2 != 0 && controler()->max_philos != 1)
		usleep(1);
	if (pthread_mutex_lock(&controler()->timer) != 0)
		return (NULL);

	philo->last_meal = current_time();
	if (!philo->last_meal)
	{
		pthread_mutex_unlock(&controler()->timer);
		return (NULL);
	}
	pthread_mutex_unlock(&controler()->timer);
	while (!stop_dinner())
	{
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
		print_logs("%s%lld %d is thinking\n", GREY, philo);
		usleep(500);
	}
	return (NULL);
}

int	create_threads(t_philosophers *philos)
{
	int			i;
	pthread_t	th;

	i = 0;
	
	// controler()->start_time = current_time();
	// if (!controler()->start_time)
	// 	return (1);
	i = 0;
	while (i < controler()->max_philos)
	{
		if (pthread_create(&philos[i].philo_th, NULL, routine, &philos[i]) != 0)
			return (printf("Error: issue with pthread create.\n"));
		i++;
	}
	if (pthread_create(&th, NULL, dead_philo, philos) != 0)
		return (printf("Error: issue with pthread create.\n"));
	while (i < controler()->max_philos)
	{
		if (pthread_join(philos[i].philo_th, NULL) != 0)
			return (printf("Error: issue with pthread join.\n"));
		i++;
	}
	if (pthread_join(th, NULL) != 0)
		return (printf("Error: issue with pthread join.\n"));
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
