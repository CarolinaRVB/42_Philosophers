/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:36:13 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/01 19:30:52 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	error_msg(char *msg, int n)
{
	if (n != -1)
		printf(msg, n);
	else
		printf("%s\n", msg);
	return (1);
}

unsigned int	ft_atoi(const char *nptr)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	res = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	return (res * sign);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*p;
	int				n;
	int				i;

	p = malloc(nmemb * size);
	if (p == NULL)
		return (NULL);
	else
	{
		i = 0;
		n = nmemb * size;
		while (n-- > 0)
			p[i++] = '\0';
	}
	return (p);
}

int	ft_isdigit_str(char *arg)
{
	if (arg)
	{
		while (*arg)
		{
			if (*arg < '0' || *arg > '9')
				return (0);
			arg++;
		}
	}
	return (1);
}

unsigned int	current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
