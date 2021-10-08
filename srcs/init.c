/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:08:04 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/08 15:52:56 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_structs(t_data **infos, t_philo **philo, char **args)
{
	*infos = malloc(sizeof(t_data));
	memset(*infos, 0, sizeof(t_data));
	(*infos)->forks_mutex = malloc(sizeof(pthread_mutex_t) * ft_atoi(args[1]));
	if (!((*infos)->forks_mutex))
		return (ERROR);
	memset((*infos)->forks_mutex, 0, sizeof(pthread_mutex_t) * \
	ft_atoi(args[1]));
	*philo = malloc(sizeof(t_philo) * ft_atoi(args[1]));
	if (!*philo)
		return (ERROR);
	memset(*philo, 0, sizeof(t_philo) * ft_atoi(args[1]));
	return (TRUE);
}

int	shared_data(t_data *infos, char **av)
{
	infos->start_time = get_start_time();
	infos->threads = malloc(sizeof(pthread_t) * ft_atoi(av[1]));
	if (!infos->threads)
		return (ERROR);
	return (TRUE);
}

void	link_philos(t_philo *philo, int i)
{
	philo->id = i + 1;
	philo->right = (i + 1) % philo->philo_number;
	philo->left = i;
}

int	init_philo(t_philo *philo, t_data *infos, int i, char **av)
{
	philo->philo_number = ft_atoi(av[1]);
	if (!is_number(av[1]) || philo->philo_number <= 0)
		return (ERROR);
	philo->time_to_die = ft_atoi(av[2]);
	if (!is_number(av[2]) || philo->time_to_die < 0)
		return (ERROR);
	philo->time_to_eat = ft_atoi(av[3]);
	if (!is_number(av[3]) || philo->time_to_eat < 0)
		return (ERROR);
	philo->time_to_sleep = ft_atoi(av[4]);
	if (!is_number(av[4]) || philo->time_to_sleep < 0)
		return (ERROR);
	if (av[5])
	{
		philo->nb_of_times_eat = ft_atoi(av[5]);
		if (!is_number(av[5]) || philo->nb_of_times_eat < 0)
			return (ERROR);
		if (philo->nb_of_times_eat == 0)
			infos->end = 1;
	}
	philo->data = infos;
	link_philos(philo, i);
	return (TRUE);
}

int	create_forks_a_philo(int i, t_data *infos, t_philo *philo, \
char **av)
{
	while (i < ft_atoi(av[1]))
	{
		memset(&philo[i], 0, sizeof(t_philo));
		if (init_philo(&philo[i], infos, i, av) == ERROR)
			return (ERROR);
		if (pthread_mutex_init(&philo->data->forks_mutex[i], NULL))
			return (print_error("Error in attempt to init mutex\n", philo));
		i++;
	}
	init_mutexes(philo);
	start_threads(philo, philo->philo_number);
	return (TRUE);
}
