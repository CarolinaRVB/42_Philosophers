/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/23 18:00:57 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*philo_routine(void *arg)
{
	t_philosopher	*philo;
	
	philo = (t_philosopher *)arg;
	if (!(philo->id % 2))
		usleep(1000);
	while (philo && !dead_philos(philo))
	{
		if (philo && dead_philos(philo))
			return (NULL);
		// if ((current_time() - philo->l_meal) >= philo->data.t_death / 1000)
		// 	return (philo_death(philo, current_time()));
		philo_grab_forks(philo);
		if (philo && dead_philos(philo))
			return (NULL);
		philo_eat(philo);
		if (philo && dead_philos(philo))
			return (NULL) ;
		philo_sleep(philo);
		if (philo && dead_philos(philo))
			return (NULL);
		print_logs("%s%lld philo %d is thinking\n", GREY, philo, current_time());
		// usleep(100);
	}
	return (NULL);
}

int	create_philosophers(t_philosopher *philo)
{
	int			i;

	i = 0;
	while (i < philo->data.n_philos)
	{
		if (pthread_create(&(philo[i].th), NULL, philo_routine, &philo[i]) != 0)
			return (error_msg("Issue in pthread create\n", -1));
		i++;
	}
	i = 0;
	if (dead_philos(philo))
		return (1);
	while (i < philo->data.n_philos)
	{
		if (pthread_join(philo[i].th, NULL) != 0)
			return (error_msg("issue in pthread join\n", -1));
		i++;
	}
	return (0);
}

void	init_philo_struct(t_philosopher *philos, t_dead *dead, char **argv)
{
	int				i;
	t_data			data;
	
	i = 0;
	data.n_philos = ft_atoll(argv[1]);
	data.t_death = ft_atoll(argv[2]) * 1000;
	data.t_eat = ft_atoll(argv[3]) * 1000;
	data.t_sleep = ft_atoll(argv[4]) * 1000;
	pthread_mutex_init(&data.print_msg, NULL);
	if (argv[5])
		philos->data.n_meals = ft_atoll(argv[5]);
	else
		philos->data.n_meals = -1;
	while (i < data.n_philos)
	{
		philos[i].n_meals = 0;
		philos[i].id = i + 1;
		philos[i].t_start = current_time();
		philos[i].l_meal = current_time();
		philos[i].dead = dead;
		philos[i].data = data;
		pthread_mutex_init(&philos[i].right_fork, NULL);
		i++;
	}
	i = 0;
	while (i < data.n_philos)
	{
		if (philos[i].id == philos->data.n_philos)
			philos[i].left_fork = &philos[0].right_fork;
		else
			philos[i].left_fork = &philos[i + 1].right_fork;
		i++;
	}
}

void	destroy_mutexes(t_philosopher *philos)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&philos->dead->dead_lock);
	pthread_mutex_destroy(&philos->dead->dead_check);
	pthread_mutex_destroy(&philos->data.print_msg);
	free(philos->dead);
	while (i < philos->data.n_philos)
	{
		pthread_mutex_destroy(&philos[i].right_fork);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_philosopher	philos[MAX];
	t_dead			*dead;

	dead = dead_check();
	if (parsing(argc, argv) != 0)
		return (1);
	init_philo_struct(philos, dead, argv);
	create_philosophers(philos);
	destroy_mutexes(philos);
	return (0);
}
