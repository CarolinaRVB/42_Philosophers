/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crebelo- <crebelo-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:21:35 by crebelo-          #+#    #+#             */
/*   Updated: 2024/04/23 18:00:42 by crebelo-         ###   ########.fr       */
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

typedef	struct s_dead
{
	int				dead;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	dead_check;
}	t_dead;

typedef struct s_data
{
	int					n_philos;
	long long			t_eat;
	long long			t_sleep;
	long long			t_death;
	long long			n_meals;
	pthread_mutex_t			print_msg;
}				t_data;

typedef struct	s_philosopher
{
	pthread_t				th;
	int						id;
	long long				t_start;
	long long				l_meal;
	int						n_meals;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			right_fork;
	t_data					data;
	t_dead					*dead;
}					t_philosopher;


/*src/main.c*/
void			*philo_routine(void *arg);
int				create_philosophers(t_philosopher *philo);
void			init_philo_struct(t_philosopher *philos, t_dead *dead, char **argv);
void			destroy_mutexes(t_philosopher *philos);
int				main(int argc, char **argv);

/*src/actions.c*/
void			philo_grab_forks(t_philosopher *philo);
void			philo_eat(t_philosopher *philo);
void			philo_sleep(t_philosopher *philo);
void			*philo_death(t_philosopher *philo, long long time);

/*src/utils1.c*/
int				error_msg(char *msg, int n);
long long int	ft_atoll(char *str);
void			*ft_calloc(size_t nmemb, size_t size);
int				ft_isdigit_str(char *arg);

/*src/utils2.c*/
int				parsing(int argc, char **argv);
t_dead			*dead_check(void);
int				dead_philos(t_philosopher *philo);
long long		current_time();
void			print_logs(char *str, char *color, t_philosopher *philo, long long time);

#endif
