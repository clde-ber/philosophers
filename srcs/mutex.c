/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:23 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/18 11:09:01 by clde-ber         ###   ########.fr       */
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

void	record_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lm_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->data->lm_mutex);
}

int	destroy_mutexes(int i, t_philo *philo)
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
	if (pthread_mutex_destroy(&philo->data->start_mutex))
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
			take_forks_right(philo);
		}
		else
		{
			pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
			pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
			take_forks_left(philo);
		}
	}
	record_last_meal(philo);
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
			pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
			pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
		}
		else
		{
			pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
			pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
		}
	}
}
