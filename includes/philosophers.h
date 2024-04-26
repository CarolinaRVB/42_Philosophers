/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:21:35 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/26 19:37:39 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define CYAN "\x1b[36m"
# define GREY "\x1b[90m"

//##############   MUTEX EXPLANATION  #######################################

/*So, even though each philosopher has its own data and resources, 
they all share the same standard output, and therefore, a mutex is used 
to synchronize access to it to avoid issues related to concurrent access.*/

//##########################################################################

typedef struct s_forks
{
	pthread_mutex_t	fork;
}	t_fork;

typedef struct s_data
{
	int				max_philos;
	int				eat_timer;
	int				sleep_timer;
	int				die_timer;
	int				max_meals;
	int				stop_dinner;
	t_fork			*forks;
	pthread_mutex_t	waiter;
	pthread_mutex_t	printer;
}	t_data;

typedef struct s_philosophers
{
	int				id;
	int				start_time;
	int				last_meal;
	int				meals_ate;
	int				rfork;
	int				lfork;
	pthread_t		philo_th;
}	t_philosophers;

/*src/main.c*/
int				cancel_dinner(t_philosophers *philo);
void			*routine(void *arg);
int				create_threads(t_philosophers *philos);
void			destroy_mutexes(t_philosophers *philos);
int				main(int argc, char **argv);

/*src/actions.c*/
int				died_while_eating(t_philosophers *philo);
int				philo_eat(t_philosophers *philo);
int				philo_sleep(t_philosophers *philo);
int				kill_philo(t_philosophers *philo);
int				stop_dinner(void);

/*src/init_dinner*/
int				init_controler(char **argv);
void			init_forks(t_fork *fork);
int				init_philos(t_philosophers *philo, char **argv);

/*src/utils1.c*/
int				error_msg(char *msg, int n);
int				ft_atoi(const char *nptr);
void			*ft_calloc(size_t nmemb, size_t size);
int				ft_isdigit_str(char *arg);

/*src/utils2.c*/
int				clean_memory(t_philosophers *philos);
t_data			*controler(void);
int				current_time(void);
int				parsing(int argc, char **argv);
int				print_logs(char *str, char *color, t_philosophers *philo);

#endif
