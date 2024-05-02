/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:51:27 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/02 08:29:50 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
	if (stop_dinner())
		return (0);
	pthread_mutex_lock(&controler()->timer);
	pthread_mutex_lock(&controler()->printer);
	// if (controler()->start_time == 0)
	// 	controler()->start_time = current_time();
	printf(str, color, current_time() - controler()->start_time, philo->id);
	pthread_mutex_unlock(&controler()->printer);
	pthread_mutex_unlock(&controler()->timer);
	return (1);
}

int	eating(t_philosophers *philo)
{
    struct timeval tv;
    unsigned int start_time, current_time;

    // // if (!print_logs("%s%d %d is eating\n", GREEN, philo))
    // //     return 0;

    if (gettimeofday(&tv, NULL))
        return 0;

    start_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

    while (1) {
        if (gettimeofday(&tv, NULL))
            return 0;

        current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

        if (current_time >= start_time + controler()->eat_timer)
            break;

        usleep(100);
    }
	philo->meals_ate++;
	pthread_mutex_lock(&controler()->meals_lock);
	if (philo->meals_ate == controler()->max_meals)
		controler()->all_philos_ate++;
	pthread_mutex_unlock(&controler()->meals_lock);
	return (1);
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
