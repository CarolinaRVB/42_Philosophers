/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:51:27 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/29 17:20:22 by crebelo-         ###   ########.fr       */
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
			return (printf("Error: invalid argument %s\n", argv[i]));
		argc--;
		i++;
	}
	if (argv[1][0] == '0' || argv[2][0] == '0'
		|| argv[3][0] == '0' || argv[4][0] == '0'
		|| (argv[5] && argv[5][0] == '0'))
		return (printf("Error: invalid argument\n"));
	return (0);
}

int	print_logs(char *str, char *color, t_philosophers *philo)
{
	pthread_mutex_lock(&controler()->printer);
	if (stop_dinner())
	{
		pthread_mutex_unlock(&controler()->printer);
		return (0);
	}
	printf(str, color, current_time() - philo->start_time, philo->id);
	pthread_mutex_unlock(&controler()->printer);
	return (1);
}

int	died_while_eating(t_philosophers *philo)
{
	int	time;

	// (void)philo;
	time = current_time();
	while (current_time() < time + philo->eat_timer)
	{
		if (stop_dinner())
		{
			pthread_mutex_unlock(&controler()->forks[philo->lfork].fork);
			pthread_mutex_unlock(&controler()->forks[philo->rfork].fork);
			return (1);
		}
		usleep(100);
	}
	return (0);
}

int	clean_memory(t_philosophers *philos)
{
	if (philos)
		free(philos);
	if (controler()->forks)
		free(controler()->forks);
	return (1);
}

t_data	*controler(void)
{
	static t_data	controler;

	return (&controler);
}
