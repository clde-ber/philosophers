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
    memset(philo, 0, sizeof(t_philo));
}

int print_error()
{
    ft_putstr_fd("Error\n", 2);
    return (ERROR);
}

int get_time(t_philo *philo)
{
    struct timeval tv;
    int ret;

    ret = 0;
    if (gettimeofday(&tv, NULL) == -1)
        return (print_error());
    ret = tv.tv_usec / 1000;
    philo->data->time += ret;
    return (ret);
}

int print_message(unsigned long time, t_philo *philo, unsigned long count)
{
    if (time == 0 || time % philo->data->time_to_die == 0)
    {
        printf("%lu milliseconds : philosopher %lu has taken a fork\n", philo->data->time, count);
        printf("%lu milliseconds : philosopher %lu is eating\n", philo->data->time, count);
        philo->data->eat_count++;
        return (TRUE);
    }
    if (time % philo->data->time_to_eat == 0)
    {
        printf("%lu milliseconds : philosopher %lu is sleeping\n", philo->data->time, count);
        return (TRUE);
    }
    if (time % philo->data->time_to_sleep == 0)
    {
        printf("%lu milliseconds : philosopher %lu is thinking\n", philo->data->time, count);
        return (TRUE);
    }
    return (FALSE);
}

void    *philo_routine(t_philo *philo)
{
    while (philo->data->eat_count < philo->data->nb_of_times_eat)
    {
        print_message(get_time(philo), philo, philo->data->philo_number);
        usleep(1000);
    }
    return (NULL);
}

void    *start_routine(void *philo)
{
    t_philo *phil;

    phil = (t_philo *)philo;
    pthread_mutex_lock(&phil->data->mutex);
    philo_routine(phil);
    phil->id++;
    pthread_mutex_unlock(&phil->data->mutex);
    return (TRUE);
}

int    start_threads(t_philo *philo, unsigned long philo_number)
{
    int ret;
    pthread_t   thread[philo_number];
    unsigned long i;
    unsigned long forks;
    
    i = 0;
    forks = philo_number;
    while (i < philo_number)
    {
        ret = pthread_create(&thread[i], NULL, start_routine, (void *)&philo[i]);
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

int main(int ac, char **av)
{
    t_philo *philo;
    t_data  *infos;
    int     i;

    i = 0;
    philo = NULL;
    infos = malloc(sizeof(t_data));
    if (!infos)
        return (print_error());
    if (ac < 5)
        return (print_error());
    else
    {
        infos->philo_number = ft_atoi(av[1]);
        infos->time_to_die = ft_atoi(av[2]);
        infos->time_to_eat = ft_atoi(av[3]);
        infos->time_to_sleep = ft_atoi(av[4]);
        if (av[5])
            infos->nb_of_times_eat = ft_atoi(av[5]);
        philo = malloc(sizeof(t_philo) * ft_atoi(av[1]));
        memset(philo, 0, sizeof(t_philo) * infos->philo_number);
        if (!philo)
            print_error();
        while (i < ft_atoi(av[1]))
        {
            philo[i].data = infos;
            i++;
        }
        i = 0;
        pthread_mutex_init(&(philo[i].data->mutex), NULL);
        start_threads(philo, philo->data->philo_number);
        pthread_mutex_destroy(&(philo[i].data->mutex));
    }
    return (0);
}