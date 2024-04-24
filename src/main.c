/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/24 19:33:52 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"



int	kill_philo(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->garçon);
	if (!controler()->stop_dinner)
	{
		controler()->stop_dinner = 1;
		print_logs("%s%d %d died\n", RED, philo, current_time());;	
	}
	pthread_mutex_unlock(&controler()->garçon);
	return (1);	
}

int	cancel_dinner(t_philosophers *philo)
{
	// int	i;

	// i = 0;
	pthread_mutex_lock(&controler()->garçon);
	if (controler()->stop_dinner || philo->meals_ate == controler()->max_meals)
	{
		pthread_mutex_unlock(&controler()->garçon);
		return (1);
	}
	if ((current_time() - philo->last_meal >= controler()->die_timer)
		|| (philo->meals_ate <= 0
		&& (current_time() - philo->start_time >= controler()->die_timer)))
	{
		pthread_mutex_unlock(&controler()->garçon);
		return (kill_philo(philo));
	}
	pthread_mutex_unlock(&controler()->garçon);
	return (0);
}

int	forks_available(t_philosophers *philo)
{
	if (!philo->fork_busy)
	{
		pthread_mutex_lock(&philo->fork);
		philo->fork_busy = 1;
		print_logs("%s%d %d has taken the right fork\n", YELLOW, philo, current_time());
	}
	else
		return (0);
	if (!philo[philo->left_fork].fork_busy)
	{
		pthread_mutex_lock(&philo[philo->left_fork].fork);
		philo[philo->left_fork].fork_busy = 1;
		print_logs("%s%d %d has taken the left fork\n", YELLOW, philo, current_time());
	}
	else
	{
		pthread_mutex_unlock(&philo->fork);
		philo->fork_busy = 0;
		return (0);
	}
	return (1);
}

int	philo_eat(t_philosophers *philo)
{
	int	time;

	time = current_time();
	print_logs("%s%d %d is eating\n", GREEN, philo, current_time());
	while (current_time() < time + controler()->eat_timer)
	{
		if (cancel_dinner(philo))
		{
			pthread_mutex_unlock(&philo[philo->left_fork].fork);
			philo[philo->left_fork].fork_busy = 0;
			pthread_mutex_unlock(&philo->fork);
			philo->fork_busy = 0;
			return (0);
		}
		usleep(100);
	}
	pthread_mutex_unlock(&philo[philo->left_fork].fork);
	philo[philo->left_fork].fork_busy = 0;
	pthread_mutex_unlock(&philo->fork);
	philo->fork_busy = 0;
	philo->last_meal = current_time();
	philo->meals_ate++;
	return (1);
}

int	philo_think(t_philosophers *philo)
{
	int	time;

	time = current_time();	
	print_logs("%s%lld %d is thinking\n", GREY, philo, current_time());
	while (current_time() < time + controler()->sleep_timer)
	{
		if (cancel_dinner(philo))
			return (0);
		usleep(100);
	}
	return (1);
}

int	philo_sleep(t_philosophers *philo)
{
	int	time;

	time = current_time();
	print_logs("%s%d %d is sleeping\n", CYAN, philo, current_time());
	while (current_time() < time + controler()->sleep_timer)
	{
		if (cancel_dinner(philo))
			return (0);
		usleep(100);
	}
	return (1);	
}

void	*routine(void *arg)
{
	t_philosophers	*philo;

	philo = (t_philosophers *)arg;
	if (philo->id % 2 != 0)
		usleep(100);
	while (!cancel_dinner(philo))
	{
		if (forks_available(philo))
		{
			if (!philo_eat(philo))
				return (NULL);
			if (!philo_sleep(philo))
				return (NULL);
		}
		if (philo_think(philo))
			return (NULL);
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
	// if (controler()->stop_dinner || cancel_dinner(philos))
	// 	return (1);
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
	printf("here at end\n");
	pthread_mutex_destroy(&controler()->printer);
	pthread_mutex_destroy(&controler()->garçon);
	while (i < controler()->max_philos)
	{
		pthread_mutex_destroy(&philos[i].fork);
		i++;
	}
}
t_data	*controler()
{
	static t_data	controler;
	
	return (&controler);
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

int	init_philos(t_philosophers *philo, char **argv)
{
	int	i;

	init_controler(argv);
	i = 0;
	while (i < controler()->max_philos)
	{
		philo[i].id = i + 1;
		philo[i].start_time = current_time();
		philo[i].last_meal = current_time();
		philo[i].meals_ate = 0;
		philo[i].fork_busy = 0;
		if (philo[i].id == controler()->max_philos)
			philo[i].left_fork = 0;
		else
			philo[i].left_fork = i + 2;
		pthread_mutex_init(&philo[i].fork, NULL);
		i++;
	}	
	return (0);
}

int	main(int argc, char **argv)
{
	t_philosophers	philos[MAX];

	if (parsing(argc, argv) != 0)
		return (1);
	init_philos(philos, argv);
	if (create_threads(philos) != 0)
		return (1);
	destroy_mutexes(philos);
	return (0);
}
