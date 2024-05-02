/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h 		                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:21:35 by crebelo-          #+#    #+#             */
/*   Updated: 2024/05/01 21:00:21 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct s_forks
{
	pthread_mutex_t	fork;
}	t_fork;

typedef struct s_data
{
	int				all_philos_ate;
	int				max_philos;
	int				stop_dinner;
	unsigned int	start_time;
	unsigned int	die_timer;
	t_fork			*forks;
	pthread_mutex_t	waiter;
	pthread_mutex_t	printer;
	pthread_mutex_t	time;
	pthread_mutex_t	meals;
	pthread_mutex_t	philo_on;
}	t_data;

typedef struct s_philosophers
{
	int				id;
	int				max_philos;
	int				max_meals;
	int				meals_ate;
	int				rfork;
	int				lfork;
	bool			on;
	unsigned int	eat_timer;
	unsigned int	sleep_timer;
	unsigned int	start_time;
	unsigned int	last_meal;
	pthread_t		philo_th;
}	t_philosophers;

/*src/main.c*/
int				cancel_dinner(t_philosophers *philo);
void			*dead_philo(void *arg);
void			*routine(void *arg);
int				create_threads(t_philosophers *philos);
int				main(int argc, char **argv);

/*src/actions.c*/
int				grab_forks(t_philosophers *philo);
int				philo_eat(t_philosophers *philo);
int				philo_sleep(t_philosophers *philo);
int				kill_philo(t_philosophers *philo);
int				stop_dinner(void);

/*src/init_dinner*/
int				init_controler(char **argv);
void			init_forks(t_fork *fork);
int				init_philos(t_philosophers *philo, char **argv);
void			destroy_mutexes(t_philosophers *philos);

/*src/utils1.c*/
int				error_msg(char *msg, int n);
unsigned int	ft_atoi(const char *nptr);
void			*ft_calloc(size_t nmemb, size_t size);
int				ft_isdigit_str(char *arg);

/*src/utils2.c*/
int				parsing(int argc, char **argv);
int				clean_memory(t_philosophers *philos);
t_data			*controler(void);
unsigned int	current_time(void);

/*src/utils3.c*/
int				took_too_long(t_philosophers *philo);
int				print_logs(char *str, t_philosophers *philo);
int				eating(t_philosophers *philo);
void			wait_to_eat(int eat, int sleep);

#endif
