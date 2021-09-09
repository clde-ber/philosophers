/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 12:35:53 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/09 18:03:16 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void     free_structs(t_philo *philo)
{
    free(philo->data->forks_mutex);
    free(philo->data->threads);
    free(philo->data);
    free(philo);
}

int    start_threads(t_philo *philo, unsigned long philo_number)
{
    int ret;
    unsigned long i;
    
    i = 0;
    while (i < philo_number)
    {
        ret = pthread_create(&philo->data->threads[i], NULL, &start_routine, (void *)&philo[i]);
        if (ret)
            return (print_error("Error in attempt to create thread\n", philo));
        i++;
    }
    i = 0;
    while (i < philo_number)
    {
        ret = pthread_join(philo->data->threads[i], NULL);
        if (ret)
            return (print_error("Error in attempt to join thread\n", philo));
        i++;
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
    free_structs(philo);
    return (0);
}
