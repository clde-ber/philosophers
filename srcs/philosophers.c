/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 12:35:53 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/09 08:46:01 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int    start_threads(t_philo *philo, unsigned long philo_number)
{
    int ret;
    unsigned long i;
    
    i = 0;
    while (i < philo_number)
    {
        ret = pthread_create(&philo->data->threads[i], NULL, start_routine, (void *)&philo[i]);
        if (ret)
            return (print_error("Error in attempt to create thread\n", philo));
        i++;
    }
    i = 0;
    while (i < philo_number)
    {
        ret = pthread_detach(philo->data->threads[i]);
        if (ret)
            return (print_error("Error in attempt to detach thread\n", philo));
        i++;
    }
    while (1)
    {
        pthread_mutex_lock(&philo->data->die_mutex);
        pthread_mutex_lock(&philo->data->count_mutex);
        if (philo->data->died || philo->eat_count >= philo->nb_of_times_eat)
        {
            pthread_mutex_unlock(&philo->data->die_mutex);
            pthread_mutex_unlock(&philo->data->count_mutex);
            break ;
        }
        pthread_mutex_unlock(&philo->data->die_mutex);
        pthread_mutex_unlock(&philo->data->count_mutex);
    }
    return (TRUE);
}

int main(int ac, char **av)
{
    t_philo *philo;
    t_data  *infos;
    unsigned long     i;

    i = 0;
    philo = NULL;
    infos = NULL;
    if (ac < 5)
        return (print_error("Error in arguments\n", NULL));
    else
    {
        init_structs(&infos, &philo, av);
        shared_data(infos, av);
        create_forks_a_philo(0, infos, philo, av);
        destroy_mutexes(0, philo);
    }
    return (0);
}
