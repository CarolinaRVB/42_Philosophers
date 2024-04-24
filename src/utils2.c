/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 10:51:27 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/24 18:39:03 by crebelo-         ###   ########.fr       */
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

int	current_time()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));	
}

void	print_logs(char *str, char *color, t_philosophers *philo, int time)
{
	(void)time;
	pthread_mutex_lock(&controler()->printer);
	printf(str, color, current_time() - philo->start_time, philo->id);
	pthread_mutex_unlock(&controler()->printer);
}
