/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:13 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/15 18:32:49 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_structs(t_data **infos, t_philo **philo, char **args)
{
	*infos = malloc(sizeof(t_data));
	memset(*infos, 0, sizeof(t_data));
	(*infos)->forks_mutex = malloc(sizeof(pthread_mutex_t) * ft_atoi(args[1]));
	if (!((*infos)->forks_mutex))
		return (print_error("Malloc error\n", NULL));
	memset((*infos)->forks_mutex, 0, sizeof(pthread_mutex_t) * \
	ft_atoi(args[1]));
	*philo = malloc(sizeof(t_philo) * ft_atoi(args[1]));
	if (!*philo)
		return (print_error("Malloc error\n", NULL));
	memset(*philo, 0, sizeof(t_philo) * ft_atoi(args[1]));
	return (TRUE);
}

int	shared_data(t_data *infos, char **av)
{
	infos->start_time = get_start_time();
	infos->threads = malloc(sizeof(pthread_t) * ft_atoi(av[1]));
	if (!infos->threads)
		print_error("Malloc error\n", NULL);
	return (TRUE);
}

void	link_philos(t_philo *philo, unsigned long i)
{
	philo->id = i + 1;
	philo->right = i + 1;
	philo->left = i;
	if (i == philo->philo_number - 1)
	{
		philo->right = 0;
		philo->left = i;
	}
}

int	init_philo(t_philo *philo, t_data *infos, unsigned long i, char **av)
{
	philo->data = infos;
	link_philos(philo, i);
	philo->philo_number = (unsigned long)ft_atoi(av[1]);
	if ((int)philo->philo_number < 2 || !is_number(av[1]))
		return (ERROR);
	philo->time_to_die = (unsigned long)ft_atoi(av[2]);
	if ((int)philo->time_to_die < 0 || !is_number(av[2]))
		return (ERROR);
	philo->time_to_eat = (unsigned long)ft_atoi(av[3]);
	if ((int)philo->time_to_eat < 0 || !is_number(av[3]))
		return (ERROR);
	philo->time_to_sleep = (unsigned long)ft_atoi(av[4]);
	if ((int)philo->time_to_sleep < 0 || !is_number(av[4]))
		return (ERROR);
	if (av[5])
	{
		philo->nb_of_times_eat = (unsigned long)ft_atoi(av[5]);
		if ((int)philo->nb_of_times_eat < 0 || !is_number(av[5]))
			return (ERROR);
		if (philo->nb_of_times_eat == 0)
			philo->data->end = 1;
	}
	return (TRUE);
}

int	create_forks_a_philo(unsigned long i, t_data *infos, t_philo *philo, \
char **av)
{
	while (i < (unsigned long)ft_atoi(av[1]))
	{
		memset(&philo[i], 0, sizeof(t_philo));
		if (init_philo(&philo[i], infos, i, av) == ERROR)
			return (ERROR);
		if (pthread_mutex_init(&philo->data->forks_mutex[i], NULL))
			return (print_error("Error in attempt to init mutex\n", philo));
		i++;
	}
	if (pthread_mutex_init(&philo->data->mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->die_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->count_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->lm_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->end_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->time_cmp_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->cumul_time_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	start_threads(philo, philo->philo_number);
	return (TRUE);
}
