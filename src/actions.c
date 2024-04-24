/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:54:42 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/23 18:01:24 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	philo_grab_forks(t_philosopher *philo)
{
	if (philo && dead_philos(philo))
		return ;
	pthread_mutex_lock(&philo->right_fork);
	if (philo && dead_philos(philo))
	{
		pthread_mutex_unlock(&philo->right_fork);
		return ;
	}
	print_logs("%s%lld %d has taken the right fork\n", YELLOW, philo, current_time());
	pthread_mutex_lock(philo->left_fork);
	if (philo && dead_philos(philo))
	{
		pthread_mutex_unlock(&philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);	
		return ;
	}
	print_logs("%s%lld %d has taken the left fork\n", YELLOW, philo, current_time());
	if (philo && dead_philos(philo))
	{
		pthread_mutex_unlock(&philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);	
		return ;
	}
}

void	philo_eat(t_philosopher *philo)
{
	print_logs("%s%lld %d is eating\n", GREEN, philo, current_time());
	usleep(philo->data.t_eat);
	philo->n_meals++;
	philo->l_meal = current_time();
	// printf("LAST MEAL: %lld\n", philo->l_meal - philo->t_start);
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	philo_sleep(t_philosopher *philo)
{
	if (dead_philos(philo))
		return ;
	print_logs("%s%lld %d is sleeping\n", CYAN, philo, current_time());
	usleep(philo->data.t_sleep);
}

void	*philo_death(t_philosopher *philo, long long time)
{
	// (void)time;
	
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	// if (dead_philos(philo))
	// 	return (NULL);
	pthread_mutex_lock(&philo->dead->dead_lock);
	// printf("time passed: %lld\n", current_time() - philo->l_meal);
	print_logs("%s%lld %d died\n", RED, philo, time);
	philo->dead->dead = 1;
	pthread_mutex_unlock(&philo->dead->dead_lock);
	return (NULL);
}
