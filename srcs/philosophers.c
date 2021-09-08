/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 12:35:53 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/08 14:59:44 by user42           ###   ########.fr       */
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
    while (!philo->data->died && philo->eat_count < philo->data->nb_of_times_eat);
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
        create_forks_a_philo(0, infos, philo);
    }
    return (0);
}
