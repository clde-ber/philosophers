/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:23 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/10 18:25:56 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	take_different_forks(t_philo *philo)
{
	if (philo->philo_number < 3)
	{
		pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
		pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
	}
	else
	{
		if (philo->id % 2)
		{
			pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
			pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
		}
		else
		{
			pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
			pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
		}
	}
}

void	release_different_forks(t_philo *philo)
{
	if (philo->philo_number < 3)
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
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
