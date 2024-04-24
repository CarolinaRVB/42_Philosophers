/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 08:36:13 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/22 11:10:32 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	error_msg(char *msg, int n)
{
	if (n != -1)
		printf(msg, n);
	else
		printf("%s\n", msg);
	return (1);
}

long long int	ft_atoll(char *str)
{
	long long int	i;
	long long int	res;
	long long int	sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 7 && str[i] <= 13))
		i++;
	while (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
		res = res * 10 + (str[i++] - '0');
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
