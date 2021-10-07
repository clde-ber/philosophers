/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 12:41:53 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/07 12:51:23 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define TRUE 0
# define FALSE 1
# define ERROR -1
# define INT_MIN -2147483648
# define INT_MAX 2147483647

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_data
{
	int				died;
	int				end;
	unsigned long	adjust;
	int				minus;
	unsigned long	start_time;
	pthread_mutex_t	mutex;
	pthread_mutex_t	die_mutex;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	count_mutex;
	pthread_mutex_t	lm_mutex;
	pthread_mutex_t	time_cmp_mutex;
	pthread_mutex_t	time_mutex;
	pthread_mutex_t	*forks_mutex;
	pthread_t		*threads;
}			t_data;

typedef struct s_philo
{
	int				philo_number;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	nb_of_times_eat;
	int				id;
	unsigned long	last_meal;
	int				right;
	int				left;
	unsigned long	eat_count;
	unsigned long	time_cmp;
	unsigned long	cumul_time;
	t_data			*data;
}			t_philo;

/*
**  init
*/

int				init_structs(t_data **infos, t_philo **philo, char **args);
int				shared_data(t_data *infos, char **av);
void			link_philos(t_philo *philo, int i);
int				init_philo(t_philo *philo, t_data *infos, int i, \
				char **av);
int				create_forks_a_philo(int i, t_data *infos, \
				t_philo *philo, char **av);

/*
**  mutex
*/

int				init_mutexes(t_philo *philo);
int				destroy_mutexes(int i, t_philo *philo);
int				take_different_forks(t_philo *philo);
void			release_different_forks(t_philo *philo);

/*
 **  philosophers
 */

void			free_structs_error(t_philo *philo, t_data *infos);
void			free_structs(t_philo *philo);
void			*start_routine(void *philo);
int				start_threads(t_philo *philo, int philo_number);

/*
**  print
*/

void			ft_putstr_fd(char *s, int fd);
void			take_forks(t_philo *philo, int is_alone);
void			print_msg(t_philo *philo, char *msg);
int				print_error(char *msg, t_philo *philo);

/*
**  routine
*/

int				philo_eat(t_philo *philo);
int				philo_sleep(t_philo *philo);
int				philo_think(t_philo *philo);
void			*philo_routine(t_philo *philo);
int				quit_routine(t_philo *philo);

/*
**  routine_utils
*/

int				single_philo_dies(t_philo *philo);
int				is_it_dead(t_philo *philo);
void			record_last_meal(t_philo *philo);

/*
**  time
*/

unsigned long	get_time(t_philo *philo);
unsigned long	get_timestamp(t_philo *philo);
unsigned long	get_start_time(void);
void			wait_action(t_philo *philo, unsigned long time);

/*
**  utils
*/

int				is_number(char *str);
int				ft_isdigit(char c);
int				ft_atoi(const char *str);

#endif
