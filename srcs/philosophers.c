/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 12:35:53 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/06 07:49:02 by clde-ber         ###   ########.fr       */
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

suseconds_t get_time(t_philo *philo)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        return (print_error());
    return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - philo->data->start_time);
}

int    philo_eat(t_philo *philo, suseconds_t time)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
        pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
    }
    else
    {
        pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
        pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
    }
    if (philo->last_meal > philo->data->time_to_die)
    {
        pthread_mutex_lock(&philo->data->mutex);
        printf("%lu milliseconds : philosopher %lu died\n", (unsigned long)philo->data->time, philo->id);
        pthread_mutex_unlock(&philo->data->mutex);
        philo->data->died = 1;
    }
    pthread_mutex_lock(&philo->data->mutex);
    printf("%lu milliseconds : philosopher %lu has taken a fork\n", (unsigned long)philo->data->time, philo->id);
    printf("%lu milliseconds : philosopher %lu is eating\n", (unsigned long)philo->data->time, philo->id);
    philo->last_meal = (unsigned long)time;
    philo->eat_count++;
    pthread_mutex_unlock(&philo->data->mutex);
    return (TRUE);
}

int    philo_sleep(t_philo *philo, suseconds_t time)
{
    (void)time;
    pthread_mutex_lock(&philo->data->mutex);
    printf("%lu milliseconds : philosopher %lu is sleeping\n", (unsigned long)philo->data->time, philo->id);
    pthread_mutex_unlock(&philo->data->mutex);
    return (TRUE);
}

int    philo_think(t_philo *philo, suseconds_t time)
{
    (void)time;
    pthread_mutex_lock(&philo->data->mutex);
    printf("%lu milliseconds : philosopher %lu is thinking\n", (unsigned long)philo->data->time, philo->id);
    pthread_mutex_unlock(&philo->data->mutex);
    return (TRUE);
}

void    *philo_routine(t_philo *philo)
{
//    int ret;

    while (!philo->data->died)
    {
        philo_eat(philo, philo->data->time += get_time(philo));
        wait_for_thread(philo, (suseconds_t)philo->data->time_to_eat);
        if (philo->id % 2 == 0)
        {
            pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
            pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
        }
        else
        {
            pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
            pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
        }
        philo_sleep(philo, philo->data->time += get_time(philo));
        wait_for_thread(philo, (suseconds_t)philo->data->time_to_sleep);
        philo_think(philo, philo->data->time += get_time(philo));
        wait_for_thread(philo, (suseconds_t)philo->data->time_to_die);
    }
    return (NULL);
}

void    *start_routine(void *philo)
{
   t_philo *phil;

    phil = (t_philo *)philo;
    pthread_mutex_lock(&phil->data->mutex);
    printf("%lu milliseconds : philosopher %lu is alive\n", (unsigned long)phil->data->time, phil->id);
    pthread_mutex_unlock(&phil->data->mutex);
    philo_routine(philo);
    (void)philo;
    return (TRUE);
}

int    start_threads(t_philo *philo, unsigned long philo_number)
{
    int ret;
    unsigned long i;
    
    i = 0;
    while (i < philo_number)
    {
        ret = pthread_create(&philo->data->threads[i], NULL, start_routine, (void *)&philo[i]);
        if (ret)
            return (print_error());
        i++;
    }
    count = 1;
    i = 0;
    while (i < philo_number)
    {
        ret = pthread_detach(philo->data->threads[i]);
        if (ret)
            return (print_error());
        i++;
    }
    i = 0;
    while (!philo->data->died);
    return (TRUE);
}

int    wait_for_thread(t_philo *philo, suseconds_t time)
{
    useconds_t time_in_micros;

    (void)philo;
    time_in_micros = time * 1000;
    if (usleep(time_in_micros) == -1)
        return (print_error());
    return (TRUE);
}

void    init_philo(t_philo *philo, t_data *infos, unsigned long i)
{
    philo->data = infos;
    philo->id = i + 1;
    if (philo->id % 2)
        philo->odd_or_even = 0;
    else
        philo->odd_or_even = 1;
    philo->eat_count = 0;
    philo->right = i + 1 % philo->data->philo_number;
    philo->left = philo->id - 1;
}

suseconds_t get_start_time()
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        return (print_error());
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void    shared_data(t_data *infos, char **av)
{
    infos->philo_number = (unsigned long)ft_atoi(av[1]);
    infos->time_to_die = (unsigned long)ft_atoi(av[2]);
    infos->time_to_eat = (unsigned long)ft_atoi(av[3]);
    infos->time_to_sleep = (unsigned long)ft_atoi(av[4]);
    if (av[5])
        infos->nb_of_times_eat = (unsigned long)ft_atoi(av[5]);
    infos->start_time = get_start_time();
    infos->threads = malloc(sizeof(pthread_t) * infos->philo_number);
    if (!infos->threads)
        print_error();
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
        infos->forks_mutex = malloc(sizeof(pthread_mutex_t) * infos->philo_number);
        if (!(infos->forks_mutex))
            print_error();
        memset(infos->forks_mutex, 0, sizeof(pthread_mutex_t) * infos->philo_number);
        philo = malloc(sizeof(t_philo) * (unsigned long)ft_atoi(av[1]));
        if (!philo)
            print_error();
        memset(philo, 0, sizeof(t_philo) * infos->philo_number);
        shared_data(infos, av);
        while (i < (unsigned long)ft_atoi(av[1]))
        {
            init_struct_philo(&philo[i]);
            init_philo(&philo[i], infos, i);
            pthread_mutex_init(&philo->data->forks_mutex[i], NULL);
            i++;
        }
        pthread_mutex_init(&philo->data->mutex, NULL);
        start_threads(philo, philo->data->philo_number);
        pthread_mutex_destroy(&philo->data->mutex);
        i = 0;
        while (i < (unsigned long)ft_atoi(av[1]))
        {
            pthread_mutex_destroy(&(philo[i].data->forks_mutex[i]));
            i++;
        }
    }
    return (0);
}