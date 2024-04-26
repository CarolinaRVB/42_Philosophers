/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:54:42 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/26 20:20:09 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	died_while_eating(t_philosophers *philo)
{
	int	time;

	time = current_time();
	while (current_time() < time + controler()->eat_timer)
	{
		if (cancel_dinner(philo))
		{
			pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
			pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
			return (1);
		}
		usleep(100);
	}
	return (0);
}

int	philo_eat(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->forks[philo->rfork].fork);
	pthread_mutex_lock(&controler()->forks[philo->lfork].fork);
	if (cancel_dinner(philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	if (!print_logs("%s%d %d is eating\n", GREEN, philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	if (died_while_eating(philo))
		return (0);
	philo->last_meal = current_time();
	philo->meals_ate++;
	pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
	pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
	return (1);
}

int	philo_sleep(t_philosophers *philo)
{
	int	time;

	if (!print_logs("%s%d %d is sleeping\n", CYAN, philo))
		return (0);
	time = current_time();
	while (current_time() < time + controler()->sleep_timer)
	{
		if (stop_dinner())
			return (1);
		usleep(100);
	}
	return (1);
}

int	kill_philo(t_philosophers *philo)
{
	if (!controler()->stop_dinner)
	{
		controler()->stop_dinner = 1;
		pthread_mutex_unlock(&controler()->waiter);
		pthread_mutex_lock(&controler()->printer);
		printf("%s%d %d died\n", RED,
			current_time() - philo->start_time, philo->id);
		pthread_mutex_unlock(&controler()->printer);
	}
	else
		pthread_mutex_unlock(&controler()->waiter);
	return (1);
}

int	stop_dinner(void)
{
	pthread_mutex_lock(&controler()->waiter);
	if (controler()->stop_dinner)
	{
		pthread_mutex_unlock(&controler()->waiter);
		return (1);
	}
	pthread_mutex_unlock(&controler()->waiter);
	return (0);
}
