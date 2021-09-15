/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:23 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/15 17:19:05 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_it_dead(t_philo *philo)
{
	if (get_time(philo) > philo->last_meal + philo->time_to_die * 1000 || \
	(philo->eat_count >= philo->nb_of_times_eat && philo->nb_of_times_eat))
		return (FALSE);
	return (TRUE);
}

int	destroy_mutexes(unsigned long i, t_philo *philo)
{
	if (pthread_mutex_destroy(&philo->data->die_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->count_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->lm_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->end_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->time_cmp_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->cumul_time_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	while (i < philo->philo_number)
	{
		if (pthread_mutex_destroy(&philo->data->forks_mutex[i]))
			return (print_error("Error in attempt to destroy mutex\n", philo));
		i++;
	}
	return (TRUE);
}

int	take_different_forks(t_philo *philo)
{
	if (is_it_dead(philo))
		return (FALSE);
	if (philo->philo_number == 2)
	{
		pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
		pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
		take_forks(philo);
	}
	else
	{
		if (philo->id % 2)
		{
			pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
			pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
			take_forks(philo);
		}
		else
		{
			pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
			pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
			take_forks(philo);
		}
	}
	pthread_mutex_lock(&philo->data->lm_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->data->lm_mutex);
	return (TRUE);
}

void	release_different_forks(t_philo *philo)
{
	if (philo->philo_number == 2)
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
	}
	else
	{
		if (philo->id % 2)
		{
			pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
			pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
		}
		else
		{
			pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
			pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
		}
	}
}
