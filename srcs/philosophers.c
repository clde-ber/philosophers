/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 12:35:53 by clde-ber          #+#    #+#             */
/*   Updated: 2021/08/25 11:46:33 by clde-ber         ###   ########.fr       */
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

unsigned long get_time(t_philo *philo)
{
    struct timeval tv;
    unsigned long ret;

    ret = 0;
    (void)philo;
    if (gettimeofday(&tv, NULL) == -1)
        return (print_error());
    if (tv.tv_usec / 1000 > 0)
        ret = tv.tv_usec / 1000;
    return (ret);
}

int print_message(unsigned long time, t_philo *philo, unsigned long count)
{
    (void)count;
    if (time == 0 || time % philo->data->time_to_die == 0)
    {
        printf("%lu milliseconds : philosopher %lu has taken a fork\n", philo->time, philo->id);
        printf("%lu milliseconds : philosopher %lu is eating\n", philo->time, philo->id);
        philo->time += time;
        philo->eat_count++;
        return (TRUE);
    }
    if (time % philo->data->time_to_eat == 0)
    {
        printf("%lu milliseconds : philosopher %lu is sleeping\n", philo->time, philo->id);
        philo->time += time;
        return (TRUE);
    }
    if (time % philo->data->time_to_sleep == 0)
    {
        printf("%lu milliseconds : philosopher %lu is thinking\n", philo->time, philo->id);
        philo->time += time;
        return (TRUE);
    }
    return (FALSE);
}

void    *philo_routine(t_philo *philo)
{
    int ret;

    while (philo->eat_count < philo->data->nb_of_times_eat)
    {
        ret = print_message(get_time(philo), philo, philo->data->philo_number);
        usleep(1000);
    }
    return (NULL);
}

void    *start_routine(void *philo)
{
    t_philo *phil;

    phil = (t_philo *)philo;
    philo_routine(phil);
    phil->data->forks = 0;
    pthread_mutex_lock(&phil->data->mutex);
    phil->data->forks = 1;
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
    while (1 && philo->eat_count < philo->data->nb_of_times_eat);
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
    unsigned long     i;

    i = 0;
    philo = NULL;
    infos = malloc(sizeof(t_data));
    if (!infos)
        return (print_error());
    if (ac < 5)
        return (print_error());
    else
    {
        memset(infos, 0, sizeof(t_data));
        infos->philo_number = (unsigned long)ft_atoi(av[1]);
        infos->time_to_die = (unsigned long)ft_atoi(av[2]);
        infos->time_to_eat = (unsigned long)ft_atoi(av[3]);
        infos->time_to_sleep = (unsigned long)ft_atoi(av[4]);
        if (av[5])
            infos->nb_of_times_eat = (unsigned long)ft_atoi(av[5]);
        philo = malloc(sizeof(t_philo) * (unsigned long)ft_atoi(av[1]));
        memset(philo, 0, sizeof(t_philo) * infos->philo_number);
        if (!philo)
            print_error();
        while (i < (unsigned long)ft_atoi(av[1]))
        {
            philo[i].data = infos;
            philo[i].id = i;
            i++;
        }
        i = 0;
        pthread_mutex_init(&(philo[i].data->mutex), NULL);
        start_threads(philo, philo->data->philo_number);
        pthread_mutex_destroy(&(philo[i].data->mutex));
    }
    return (0);
}