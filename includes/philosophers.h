/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:21:35 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/26 17:07:43 by crebelo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

#define MAX 201
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define CYAN "\x1b[36m"
#define GREY "\x1b[90m"

//##############   MUTEX EXPLANATION  #######################################

/*So, even though each philosopher has its own data and resources, 
they all share the same standard output, and therefore, a mutex is used 
to synchronize access to it to avoid issues related to concurrent access.*/

//##########################################################################

//in the t_philosopher.t_data structure, the time for each activity needs to 
//be decremented for each of the philosophers

typedef struct s_forks
{
	pthread_mutex_t	fork;
}	t_fork;

typedef struct	s_data
{
	int				max_philos;
	int				eat_timer;
	int				sleep_timer;
	int				die_timer;
	int				max_meals;
	int				stop_dinner;
	t_fork			*forks;
	pthread_mutex_t	garçon;
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


int	stop_dinner();
/*src/main.c*/
// void			*philo_routine(void *arg);
// int				create_philosophers(t_philosopher *philo);
// void			init_philo_struct(t_philosopher *philos, t_dead *dead, char **argv);
// void			destroy_mutexes(t_philosopher *philos);
int				main(int argc, char **argv);

/*src/actions.c*/
// void			philo_grab_forks(t_philosopher *philo);
int				philo_eat(t_philosophers *philo);
int				philo_sleep(t_philosophers *philo);
int				kill_philo(t_philosophers *philo);
int				stop_dinner();

/*src/utils1.c*/
int				error_msg(char *msg, int n);
void			*ft_calloc(size_t nmemb, size_t size);
int				ft_isdigit_str(char *arg);
int				ft_atoi(const char *nptr);

/*src/utils2.c*/
t_data			*controler();
int				parsing(int argc, char **argv);
int				current_time();
int				print_logs(char *str, char *color, t_philosophers *philo, int time);

#endif
