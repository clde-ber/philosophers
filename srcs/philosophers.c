/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 12:35:53 by clde-ber          #+#    #+#             */
/*   Updated: 2021/08/27 21:29:27 by clde-ber         ###   ########.fr       */
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
    ret = (tv.tv_usec - philo->data->start_time) / 1000;
    return (ret);
}

int print_message(unsigned long time, t_philo *philo, unsigned long count)
{
    (void)count;
    if (time == 0 || time % philo->data->time_to_die == 0)
    {
        printf("%lu milliseconds : philosopher %lu has taken a fork\n", philo->data->time, philo->id);
        printf("%lu milliseconds : philosopher %lu is eating\n", philo->data->time, philo->id);
        philo->eat_count++;
        philo->data->time += time;
        return (TRUE);
    }
    if (time % philo->data->time_to_eat == 0)
    {
        philo->data->forks = 0;
        printf("%lu milliseconds : philosopher %lu is sleeping\n", philo->data->time, philo->id);
        philo->data->time += time;
        return (TRUE);
    }
    if (time % philo->data->time_to_sleep == 0)
    {
        printf("%lu milliseconds : philosopher %lu is thinking\n", philo->data->time, philo->id);
        philo->data->time += time;
        return (TRUE);
    }
    return (FALSE);
}

int    philo_eat(t_philo *philo, unsigned long time)
{
    if (time % philo->data->time_to_die == 0)
    {
        printf("%lu milliseconds : philosopher %lu has taken a fork\n", philo->data->time, philo->id);
        printf("%lu milliseconds : philosopher %lu is eating\n", philo->data->time, philo->id);
        philo->data->time += time;
        philo->data->forks = 1;
        philo->eat_count++;
        return (TRUE);
    }
    return (FALSE);
}

int    philo_sleep(t_philo *philo, unsigned long time)
{
    if (time % philo->data->time_to_eat == 0)
    {
    /*    philo->data->forks = 0;
        pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
        pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);*/
        printf("%lu milliseconds : philosopher %lu is sleeping\n", philo->data->time, philo->id);
        philo->data->time += time;
        return (TRUE);
    }
    return (FALSE);
}

int    philo_think(t_philo *philo, unsigned long time)
{
    if (time % philo->data->time_to_sleep == 0)
    {
        printf("%lu milliseconds : philosopher %lu is thinking\n", philo->data->time, philo->id);
        philo->data->time += time;
        return (TRUE);
    }
    return (FALSE);
}

void    *philo_routine(t_philo *philo)
{
//    int ret;
    unsigned long time;

    time = 0;
    while (!philo->data->died)
    {
        time = get_time(philo);
        if (!philo_eat(philo, time))
            wait_for_thread(philo, philo->data->time_to_eat);
        if (!philo_sleep(philo, time))
            wait_for_thread(philo, philo->data->time_to_sleep);
        if (!philo_think(philo, time))
            wait_for_thread(philo, philo->data->time_to_die);
    }
    return (NULL);
}

void    *start_routine(void *philo)
{
   t_philo *phil;

    phil = (t_philo *)philo;
    philo_routine(phil);
    (void)philo;
    return (TRUE);
}

int    start_threads(t_philo *philo, unsigned long philo_number)
{
    int ret;
    unsigned long i;
    unsigned long forks;
    
    i = 0;
    forks = philo_number;
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
    while (!philo->data->died);
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

void    init_philo(t_philo *philo, t_data *infos, unsigned long i)
{
    philo->data = infos;
    philo->id = i + 1;
    if (philo->id % 2)
        philo->odd_or_even = 0;
    else
        philo->odd_or_even = 1;
    philo->eat_count = 0;
    if (philo->id < philo->data->philo_number)
        philo->right = philo->id;
    else
        philo->right = 0;
    philo->left = i;
}

unsigned long get_start_time()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_usec);
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
        printf("avant\n");
        shared_data(infos, av);
        while (i < (unsigned long)ft_atoi(av[1]))
        {
            init_philo(&philo[i], infos, i);
            pthread_mutex_init(&philo->data->forks_mutex[i], NULL);
            i++;
        }
        printf("apres\n");
        i = 0;
  //      pthread_mutex_init(&(philo[i].data->mutex), NULL);
        start_threads(philo, philo->data->philo_number);
  //      pthread_mutex_destroy(&(philo[i].data->mutex));
        i = 0;
        while (i < (unsigned long)ft_atoi(av[1]))
        {
            pthread_mutex_destroy(&(philo[i].data->forks_mutex[i]));
            i++;
        }
    }
    return (0);
}