/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:54:42 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/01 18:05:22 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	grab_forks(t_philosophers *philo)
{
	if (stop_dinner()
		|| pthread_mutex_lock(&controler()->forks[philo->rfork].fork) != 0)
		return (0);
	if (!print_logs("%s%d %d has taken a fork\n", YELLOW, philo)
		|| controler()->max_philos == 1)
	{
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	if (stop_dinner()
		|| pthread_mutex_lock(&controler()->forks[philo->lfork].fork) != 0)
	{
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	if (!print_logs("%s%d %d has taken a fork\n", YELLOW, philo))
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
	if (pthread_mutex_lock(&controler()->timer) != 0)
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		return (0);
	}
	philo->last_meal = current_time();
	if (!philo->last_meal)
	{
		pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
		pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
		pthread_mutex_unlock(&controler()->timer);
		return (0);
	}
	pthread_mutex_unlock(&controler()->timer);
	pthread_mutex_unlock(&controler()->waiter);
	if (!eating(philo))
		return (0);
	pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
	pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
	return (1);
}

int	philo_sleep(t_philosophers *philo)
{
    struct timeval tv;
    unsigned int start_time, current_time;

    if (!print_logs("%s%d %d is sleeping\n", CYAN, philo))
        return 0;

    if (gettimeofday(&tv, NULL))
        return 0;

    start_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

    while (1) 
	{
        if (gettimeofday(&tv, NULL))
            return 0;

        current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

        if (current_time >= start_time + controler()->sleep_timer)
            break;

        usleep(100);
	}
	// time = current_time();
	// if (!time)
	// 	return (0);
	// while (current_time() < time + controler()->sleep_timer)
	// 	usleep(100);
	return (1);
}

int	kill_philo(t_philosophers *philo)
{
	if (pthread_mutex_lock(&controler()->waiter) != 0)
		return (1);
	if (!controler()->stop_dinner)
	{
		controler()->stop_dinner = 1;
		pthread_mutex_unlock(&controler()->waiter);
		if (pthread_mutex_lock(&controler()->printer) != 0)
			return (1);
		
		pthread_mutex_lock(&controler()->timer);
		printf("%s%d %d died\n", RED,
			current_time() - controler()->start_time, philo->id);
		
		pthread_mutex_unlock(&controler()->timer);
		pthread_mutex_unlock(&controler()->printer);
	}
	else
		pthread_mutex_unlock(&controler()->waiter);
	return (1);
}

int	stop_dinner(void)
{
	if (pthread_mutex_lock(&controler()->waiter) != 0)
		return (1);
	if (controler()->stop_dinner)
	{
		pthread_mutex_unlock(&controler()->waiter);
		return (1);
	}
	pthread_mutex_unlock(&controler()->waiter);
	return (0);
}
