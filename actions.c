/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:54:42 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/02 20:34:00 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	grab_forks(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->forks[philo->rfork].fork);
	if (!print_logs("%d %d has taken a fork\n", philo)
		|| controler()->max_philos == 1)
	{
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	pthread_mutex_lock(&controler()->forks[philo->lfork].fork);
	if (!print_logs("%d %d has taken a fork\n", philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	return (1);
}

int	philo_eat(t_philosophers *philo)
{
	if (!grab_forks(philo))
		return (0);
	if (!print_logs("%d %d is eating\n", philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	pthread_mutex_lock(&controler()->time);
	philo->last_meal = current_time();
	pthread_mutex_unlock(&controler()->time);
	if (eating(philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	philo->meals_ate++;
	pthread_mutex_lock(&controler()->meals);
	if (philo->meals_ate == philo->max_meals)
		controler()->all_philos_ate++;
	pthread_mutex_unlock(&controler()->meals);
	return (1);
}

int	philo_sleep(t_philosophers *philo)
{
	struct timeval	tv;
	unsigned int	start_time;
	unsigned int	current_time;

	if (!print_logs("%d %d is sleeping\n", philo))
		return (0);
	if (gettimeofday(&tv, NULL))
		return (0);
	start_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	while (1) 
	{
		if (gettimeofday(&tv, NULL))
			return (0);
		current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
		if (current_time >= start_time + philo->sleep_timer)
			break ;
		usleep(100);
	}
	return (1);
}

int	kill_philo(t_philosophers *philo)
{
	unsigned int	fixed_time;

	pthread_mutex_lock(&controler()->waiter);
	if (!controler()->stop_dinner)
	{
		controler()->stop_dinner = 1;
		pthread_mutex_unlock(&controler()->waiter);
		pthread_mutex_lock(&controler()->printer);
		pthread_mutex_lock(&controler()->time);
		fixed_time = controler()->start_time - philo->start_time;
		printf("%d %d died\n",
			(current_time() - (philo->start_time - fixed_time)), philo->id);
		pthread_mutex_unlock(&controler()->time);
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
