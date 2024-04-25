/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/25 01:37:09 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	kill_philo(t_philosophers *philo)
{
	// pthread_mutex_lock(&controler()->garçon);
	if (!controler()->stop_dinner)
	{
		print_logs("%s%d %d died\n", RED, philo, current_time());
		controler()->stop_dinner = 1;
	}
	pthread_mutex_unlock(&controler()->garçon);
	return (1);	
}

int	cancel_dinner(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->garçon);
	usleep(1);
	if ((philo->meals_ate <= 0
		&& (current_time() - philo->start_time >= controler()->die_timer)) || (philo->meals_ate <= 0 && (current_time() - philo->last_meal >= controler()->die_timer)))
		return (kill_philo(philo));
	if (controler()->stop_dinner || philo->meals_ate == controler()->max_meals)
	{
		pthread_mutex_unlock(&controler()->garçon);
		return (1);
	}
	pthread_mutex_unlock(&controler()->garçon);
	return (0);
}

int	forks_available(t_philosophers *philo)
{
	if (!cancel_dinner(philo) && !controler()->forks[philo->rfork].status)
	{
		print_logs("%s%d %d has taken the right fork\n", YELLOW, philo, current_time());
		pthread_mutex_lock(&controler()->forks[philo->rfork].fork);
		controler()->forks[philo->rfork].status = 1;
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
	}
	else
		return (0);
	if (!cancel_dinner(philo) && !controler()->forks[philo->lfork].status)
	{
		print_logs("%s%d %d has taken the left fork\n", YELLOW, philo, current_time());
		pthread_mutex_lock(&controler()->forks[philo->lfork].fork);
		controler()->forks[philo->lfork].status = 1;
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
	}
	else
	{
		pthread_mutex_lock(&controler()->forks[philo->rfork].fork);
		controler()->forks[philo->rfork].status = 0;
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	return (1);
}

int	philo_eat(t_philosophers *philo)
{
	int	time;

	print_logs("%s%d %d is eating\n", GREEN, philo, current_time());
	time = current_time();
	while (!controler()->stop_dinner && current_time() < time + controler()->eat_timer)
	{
		if (cancel_dinner(philo))
		{
			pthread_mutex_lock(&controler()->forks[philo->lfork].fork);
			controler()->forks[philo->lfork].status = 0;
			pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
			pthread_mutex_lock(&controler()->forks[philo->rfork].fork);
			controler()->forks[philo->rfork].status = 0;
			pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
			return (0);
		}
	}
	pthread_mutex_lock(&controler()->forks[philo->lfork].fork);
	controler()->forks[philo->lfork].status = 0;
	pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
	pthread_mutex_lock(&controler()->forks[philo->rfork].fork);
	controler()->forks[philo->rfork].status = 0;
	pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
	philo->last_meal = current_time();
	philo->meals_ate++;
	return (1);
}

int	philo_think(t_philosophers *philo)
{
	int	time;

	print_logs("%s%lld %d is thinking\n", GREY, philo, current_time());
	time = current_time();	
	while (current_time() < time + controler()->sleep_timer)
	{
		if (cancel_dinner(philo))
			return (0);
	}
	return (1);
}

int	philo_sleep(t_philosophers *philo)
{
	int	time;

	print_logs("%s%d %d is sleeping\n", CYAN, philo, current_time());
	time = current_time();
	while (!controler()->stop_dinner && current_time() < time + controler()->sleep_timer)
	{
		if (cancel_dinner(philo))
			return (0);
	}
	return (1);	
}

void	*routine(void *arg)
{
	t_philosophers	*philo;

	philo = (t_philosophers *)arg;
	if (philo->id % 2 != 0)
		usleep(100);
	while (!controler()->stop_dinner && !cancel_dinner(philo))
	{
		if (forks_available(philo))
		{
			if (!philo_eat(philo))
				return (NULL);
			if (!philo_sleep(philo))
				return (NULL);
		}
		if (controler()->stop_dinner || cancel_dinner(philo))
			return (NULL);
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

void	destroy_mutexes(void)
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

// instead of starting with even numbers i can use the init forks to
// lock the odd forks

void	init_forks(t_fork *fork)
{
	int	i;

	i = 0;
	while (i < controler()->max_philos)
	{
		// fork[i].id = i + 1;
		fork[i].status = 0;
		pthread_mutex_init(&fork[i].fork, NULL);
		i++;
	}
}

int	init_philos(t_philosophers *philo, char **argv)
{
	int	i;
	t_fork	fork[MAX];

	init_controler(argv);
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
			philo[i].lfork = 0;
		else
			philo[i].lfork = i + 1;
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
	destroy_mutexes();
	return (0);
}