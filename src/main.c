/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/26 17:06:09 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	cancel_dinner(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->garçon);
	if ((current_time() - philo->last_meal >= controler()->die_timer) || (philo->meals_ate <= 0
		&& ((current_time() - philo->start_time >= controler()->die_timer))))
		return (kill_philo(philo));
	if (philo->meals_ate == controler()->max_meals)
	{
		controler()->stop_dinner = 1;
		pthread_mutex_unlock(&controler()->garçon);
		return (1);
	}
	pthread_mutex_unlock(&controler()->garçon);
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
		print_logs("%s%lld %d is thinking\n", GREY, philo, current_time());
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
			return(printf("Error: issue with thread\n"));
		i++;
	}
	i = 0;
	while (i < controler()->max_philos)
	{
		if (pthread_join(philos[i].philo_th, NULL) != 0)
			return(printf("Error: issue with thread\n"));
		i++;
	}
	return (0);
}

void	destroy_mutexes(t_philosophers *philos)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&controler()->printer);
	pthread_mutex_destroy(&controler()->garçon);
	while (i < controler()->max_philos)
	{
		pthread_mutex_destroy(&controler()->forks[i].fork);
		i++;
	}
	free(controler()->forks);
	free(philos);
}

void	init_controler(char **argv)
{
	controler()->max_philos = ft_atoi(argv[1]);
	controler()->die_timer = ft_atoi(argv[2]);
	controler()->eat_timer = ft_atoi(argv[3]);
	controler()->sleep_timer = ft_atoi(argv[4]);
	if (argv[5])
		controler()->max_meals = ft_atoi(argv[5]);
	else
		controler()->max_meals = -1;
	controler()->stop_dinner = 0;
	pthread_mutex_init(&controler()->garçon, NULL);
	pthread_mutex_init(&controler()->printer, NULL);
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
	t_fork		*fork;

	init_controler(argv);
	fork = malloc(controler()->max_philos * sizeof(t_fork));
	if (!fork)
		return (1);
	init_forks(fork);
	controler()->forks = fork;
	i = 0;
	while (i < controler()->max_philos)
	{
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
	return (0);
}

int	main(int argc, char **argv)
{
	t_philosophers *philos;

	if (parsing(argc, argv) != 0)
		return (1);
	philos = malloc(ft_atoi(argv[1]) * sizeof(t_philosophers));
	if (!philos)
		return (1);
	init_philos(philos, argv);
	if (create_threads(philos) != 0)
		return (1);
	destroy_mutexes(philos);
	return (0);
}
