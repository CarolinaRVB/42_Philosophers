/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:19:10 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/02 21:23:38 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	took_too_long(t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->time);
	if ((current_time() - philo->last_meal >= controler()->die_timer))
	{
		pthread_mutex_unlock(&controler()->time);
		kill_philo(philo);
		return (1);
	}
	pthread_mutex_unlock(&controler()->time);
	return (0);
}

int	print_logs(char *str, t_philosophers *philo)
{
	unsigned int	fixed_time;

	if (stop_dinner())
		return (0);
	pthread_mutex_lock(&controler()->printer);
	pthread_mutex_lock(&controler()->time);
	fixed_time = controler()->start_time - philo->start_time;
	printf(str, current_time() 
		- (philo->start_time - fixed_time), philo->id);
	pthread_mutex_unlock(&controler()->time);
	pthread_mutex_unlock(&controler()->printer);
	return (1);
}

int	eating(t_philosophers *philo)
{
	struct timeval	tv;
	unsigned int	start_time;
	unsigned int	current_time;

	if (!gettimeofday(&tv, NULL))
	{
		start_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
		while (1) 
		{
			if (gettimeofday(&tv, NULL))
				return (1);
			current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
			if (current_time >= start_time + philo->eat_timer)
				break ;
			usleep(100);
		}
	}
	else
		return (1);
	pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
	pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
	return (0);
}

void	wait_to_eat(int eat, int sleep)
{
	int	res;

	res = eat - sleep;
	if (res < 0)
		usleep(500);
	else
		usleep(res + 500);
}
