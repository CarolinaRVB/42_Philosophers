/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:51:27 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/23 18:00:38 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	parsing(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
		return (error_msg("Error: args: required: 5-6, provided: %d\n", argc));
	while (argc)
	{
		if (!ft_isdigit_str(argv[i]))
			return (error_msg("Error: invalid arg %d\n", ft_atoll(argv[i])));
		argc--;
		i++;
	}
	if (ft_atoll(argv[1]) > 200 || argv[1][0] == '0' || argv[2][0] == '0'
		|| argv[3][0] == '0' || argv[4][0] == '0')
		return (error_msg("Error: invalid arg number\n", -1));
	return (0);
}

t_dead	*dead_check(void)
{
	static t_dead	*dead_flag;
	
	dead_flag = malloc(sizeof(t_dead));
	dead_flag->dead = 0;
	pthread_mutex_init(&dead_flag->dead_lock, NULL);
	pthread_mutex_init(&dead_flag->dead_check, NULL);
	return (dead_flag);
}

int	dead_philos(t_philosopher *philo)
{
	int i;

	i = 0;
	// if (philo && philo[i].id && philo[i].dead && philo[i].data.n_philos)
	// {
		pthread_mutex_lock(&philo->dead->dead_check);
		// usleep(10);
		while (i < philo[i].data.n_philos)
		{
			if (philo[i].dead && philo[i].dead->dead)
			{
				pthread_mutex_unlock(&philo->dead->dead_check);
				return (1);	
			}
			if (!philo[i].n_meals && (current_time() - philo[i].t_start >= philo->data.t_death / 1000))
			{
				philo_death(philo, current_time());
				// pthread_mutex_lock(&philo->dead->dead_lock);
				// print_logs("%s%lld %d died\n", RED, philo, current_time());
				// philo[i].dead->dead = 1;
				// pthread_mutex_unlock(&philo->dead->dead_lock);
				pthread_mutex_unlock(&philo->dead->dead_check);
				return (1);
			}
			if ((current_time() - philo[i].l_meal) >= philo->data.t_death / 1000)
			{
				philo_death(philo, current_time());
				// pthread_mutex_lock(&philo->dead->dead_lock);
				// print_logs("%s%lld %d died\n", RED, philo, current_time());
				// philo[i].dead->dead = 1;
				// pthread_mutex_unlock(&philo->dead->dead_lock);
				pthread_mutex_unlock(&philo->dead->dead_check);
				return (1);	
			}
			i++;
		}
		pthread_mutex_unlock(&philo->dead->dead_check);

	// }
	return (0);
}

long long	current_time()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));	
}

void	print_logs(char *str, char *color, t_philosopher *philo, long long time)
{
	pthread_mutex_lock(&philo->data.print_msg);
	printf(str, color, time - philo->t_start, philo->id);
	pthread_mutex_unlock(&philo->data.print_msg);
}
