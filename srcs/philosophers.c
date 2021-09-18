/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:35 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/18 11:08:51 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_structs_error(t_philo *philo, t_data *infos)
{
	free(infos->threads);
	free(infos->forks_mutex);
	free(infos);
	free(philo);
}

void	free_structs(t_philo *philo)
{
	free(philo->data->forks_mutex);
	free(philo->data->threads);
	free(philo->data);
	free(philo);
}

void	*start_routine(void *philo)
{
	t_philo	*phil;
	int		index;

	phil = (t_philo *)philo;
	pthread_mutex_lock(&phil->data->start_mutex);
	index = phil->data->start++;
	pthread_mutex_lock(&phil->data->mutex);
	print_msg(philo, "%lu milliseconds : philosopher %lu is alive\n");
	pthread_mutex_unlock(&phil->data->mutex);
	pthread_mutex_unlock(&phil->data->start_mutex);
	while (index < phil->philo_number)
	{
		pthread_mutex_lock(&phil->data->start_mutex);
		index = phil->data->start;
		pthread_mutex_unlock(&phil->data->start_mutex);
	}
	philo_routine(philo);
	return (TRUE);
}

int	start_threads(t_philo *philo, int philo_number)
{
	int	ret;
	int	i;

	i = 0;
	while (i < philo_number)
	{
		ret = pthread_create(&philo->data->threads[i], NULL, &start_routine, \
		(void *)&philo[i]);
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

int	main(int ac, char **av)
{
	t_philo			*philo;
	t_data			*infos;

	philo = NULL;
	infos = NULL;
	if (ac < 5)
		return (print_error("Error in arguments\n", NULL));
	else
	{
		init_structs(&infos, &philo, av);
		shared_data(infos, av);
		if (ft_atoi(av[1]) <= 0 || \
		create_forks_a_philo(0, infos, philo, av) == ERROR)
		{
			free_structs_error(philo, infos);
			return (print_error("Error in arguments", NULL));
		}
		destroy_mutexes(0, philo);
	}
	free_structs(philo);
	return (0);
}
