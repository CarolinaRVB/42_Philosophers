/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:54:42 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/26 17:04:22 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	philo_eat(t_philosophers *philo)
{
	int	time;

	pthread_mutex_lock(&controler()->forks[philo->rfork].fork);
	pthread_mutex_lock(&controler()->forks[philo->lfork].fork);
	if (cancel_dinner(philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	if (!print_logs("%s%d %d is eating\n", GREEN, philo, current_time()))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	time = current_time();
	while (current_time() < time + controler()->eat_timer)
	{
		if (cancel_dinner(philo))
		{
			pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
			pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
			return (0);
		}
		usleep(100);
	}
	philo->last_meal = current_time();
	philo->meals_ate++;
	pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
	pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
	return (1);
}

int	philo_sleep(t_philosophers *philo)
{
	int	time;

	if (!print_logs("%s%d %d is sleeping\n", CYAN, philo, current_time()))
		return (0);
	time = current_time();
	usleep(controler()->sleep_timer * 1000);
	return (1);	
}

int	kill_philo(t_philosophers *philo)
{
	if (!controler()->stop_dinner)
	{
		controler()->stop_dinner = 1;
		pthread_mutex_unlock(&controler()->garçon);
		pthread_mutex_lock(&controler()->printer);
		printf("%s%d %d died\n", RED, current_time() - philo->start_time, philo->id);
		pthread_mutex_unlock(&controler()->printer);
	}
	else
		pthread_mutex_unlock(&controler()->garçon);
	return (1);	
}

int	stop_dinner()
{
	pthread_mutex_lock(&controler()->garçon);
	if (controler()->stop_dinner)
	{
		pthread_mutex_unlock(&controler()->garçon);
		return (1);
	}
	pthread_mutex_unlock(&controler()->garçon);
	return (0);
}