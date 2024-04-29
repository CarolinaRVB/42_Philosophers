/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:54:42 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/29 09:58:58 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	grab_forks(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->forks[philo->rfork].fork);
	if (!print_logs("%s%d %d has taken a fork\n", YELLOW, philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}	
	pthread_mutex_lock(&controler()->forks[philo->lfork].fork);
	if (!print_logs("%s%d %d has taken a fork\n", YELLOW, philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}	
	if (cancel_dinner(philo))
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
	if (!print_logs("%s%d %d is eating\n", GREEN, philo))
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	philo->last_meal = current_time();
	if (died_while_eating(philo))
		return (0);
	philo->meals_ate++;
	if (philo->meals_ate == controler()->max_meals)
		controler()->all_philos_ate++;
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
		// if (stop_dinner())
		// 	return (0);
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
