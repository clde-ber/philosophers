/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 12:35:53 by clde-ber          #+#    #+#             */
/*   Updated: 2021/08/23 12:07:20 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static unsigned long count = 1;

void    init_struct_philo(t_philo *philo)
{
    struct timeval tv;
    
    philo->philo_number = 0;
    philo->philo_count = 0;
    philo->time_to_die = 0;
    philo->time_to_eat = 0;
    philo->time_to_sleep = 0;
    philo->nb_of_times_eat = 0;
    philo->odd_or_even = 0;
    philo->time = 0;
    philo->eat_count = 0;
    philo->count = 1;
    philo->ret = 0;
    if (gettimeofday(&tv, NULL) == -1)
        philo->seventies_value = 0;
    philo->seventies_value = tv.tv_usec;
}

int print_error()
{
    ft_putstr_fd("Error\n", 2);
    return (ERROR);
}

int get_time(t_philo *philo)
{
    struct timeval tv;

    (void)philo;
    if (gettimeofday(&tv, NULL) == -1)
        return (print_error());
    //printf("tv.tv_usec %lu\n", tv.tv_usec);
  //  printf("philo->seventies_value %lu\n", philo->seventies_value);
    philo->ret = tv.tv_usec / 1000;
    return (TRUE);
}

int print_message(unsigned long time, t_philo *philo, unsigned long count)
{
    if (time == 0 || time % (((t_philo *)philo)->time_to_die) == 0)
    {
        printf("%lu milliseconds : philosopher %lu has taken a fork\n", ((t_philo *)philo)->time, count);
        printf("%lu milliseconds : philosopher %lu is eating\n", ((t_philo *)philo)->time, count);
        philo->time += time;
        philo->eat_count++;
        return (TRUE);
    }
    if (time % (((t_philo *)philo)->time_to_eat) == 0)
    {
        printf("%lu milliseconds : philosopher %lu is sleeping\n", ((t_philo *)philo)->time, count);
        philo->time += time;
        return (TRUE);
    }
    if (time % (((t_philo *)philo)->time_to_sleep) == 0)
    {
        printf("%lu milliseconds : philosopher %lu is thinking\n", ((t_philo *)philo)->time, count);
        philo->time += time;
        return (TRUE);
    }
    return (FALSE);
}

/*get_time(philo);
    if (((t_philo *)philo)->ret == 0 || ((t_philo *)philo)->ret % (((t_philo *)philo)->time_to_die) == 0)
    {
        printf("%lu milliseconds : philosopher %lu has taken a fork\n", ((t_philo *)philo)->time, count);
        printf("%lu milliseconds : philosopher %lu is eating\n", ((t_philo *)philo)->time, count);
        ((t_philo *)philo)->time += ((t_philo *)philo)->ret;
    }
    if (((t_philo *)philo)->time % (((t_philo *)philo)->time_to_eat) == 0)
    {
        printf("%lu milliseconds : philosopher %lu is sleeping\n", ((t_philo *)philo)->time, count);
        ((t_philo *)philo)->time += ((t_philo *)philo)->ret;
    }
    if (((t_philo *)philo)->time % (((t_philo *)philo)->time_to_sleep) == 0)
    {
        printf("%lu milliseconds : philosopher %lu is thinking\n", ((t_philo *)philo)->time, count);
        ((t_philo *)philo)->time += ((t_philo *)philo)->ret;
    }
    usleep(1000);*/

void    *start_routine(void *philo)
{
    t_philo *phil;

    phil = (t_philo *)philo;
    pthread_mutex_lock(&phil->mutex);
    phil->count++;
    pthread_mutex_unlock(&phil->mutex);
    return (TRUE);
}

int    take_fork(t_philo *philo, unsigned long philo_number)
{
    int ret;
    pthread_t   thread[philo_number + 1];
    unsigned long i;
    void *code;
    unsigned long forks;
    
    i = 0;
    code = 0;
    thread[philo_number] = 0;
    forks = philo_number + 1;
    while (i < philo_number)
    {
        ret = pthread_create(&thread[i], NULL, start_routine, (void *)philo);
        if (ret)
            return (print_error());
        i++;
    }
    i = 0;
    count = 1;
    while (i < philo_number)
    {
        ret = pthread_detach(thread[i]);
        if (ret)
            return (print_error());
        i++;
    }
    count = 1;
    while (philo->eat_count < philo->nb_of_times_eat)
    {
        while (print_message(philo->ret, philo, philo->count) == FALSE)
        {
            get_time(philo);
            usleep(1000);
        }
    }
    return (TRUE);
}

int    wait_for_thread(t_philo *philo, unsigned long time)
{
    unsigned long time_in_micros;

    (void)philo;
    time_in_micros = time * 1000;
    if (usleep(time_in_micros) == -1)
        return (print_error());
    return (TRUE);
}

void    start_threads(t_philo *philo)
{
    unsigned long forks;
    
    forks = philo->philo_number;
    take_fork(philo, philo->philo_number);
}

int main(int ac, char **av)
{
    t_philo philo;
    int     i;

    i = 0;
    init_struct_philo(&philo);
    pthread_mutex_init(&philo.mutex, NULL);
    if (ac < 5)
        return (print_error());
    else
    {
        philo.philo_number = ft_atoi(av[1]);
        philo.philo_count = philo.philo_number;
        philo.time_to_die = ft_atoi(av[2]);
        philo.time_to_eat = ft_atoi(av[3]);
        philo.time_to_sleep = ft_atoi(av[4]);
        if (av[5])
            philo.nb_of_times_eat = ft_atoi(av[5]);
    }
    start_threads(&philo);
    pthread_mutex_destroy(&philo.mutex);
    return (0);
}