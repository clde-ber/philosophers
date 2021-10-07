/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:13 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/05 16:00:44 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	quit_routine(t_philo *philo)
{
	if ((philo->eat_count < philo->nb_of_times_eat && philo->nb_of_times_eat) \
	|| !philo->nb_of_times_eat)
	{
		pthread_mutex_lock(&philo->data->mutex);
		pthread_mutex_lock(&philo->data->die_mutex);
		printf("%lu milliseconds : philosopher %d died\n", get_time(philo), philo->id);
		philo->data->died = 1;
		pthread_mutex_unlock(&philo->data->mutex);
		pthread_mutex_unlock(&philo->data->die_mutex);
	}
	else
	{
		pthread_mutex_lock(&philo->data->end_mutex);
		philo->data->end = 1;
		pthread_mutex_unlock(&philo->data->end_mutex);
	}
	return (FALSE);
}

int	philo_eat(t_philo *philo)
{
	if (take_different_forks(philo))
		return (quit_routine(philo));
	if (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu milliseconds : philosopher %d is eating\n");
		pthread_mutex_unlock(&philo->data->mutex);
		pthread_mutex_lock(&philo->data->count_mutex);
		philo->eat_count++;
		pthread_mutex_unlock(&philo->data->count_mutex);
	}
	return (TRUE);
}

int	philo_sleep(t_philo *philo)
{
	if (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu milliseconds : philosopher %d is sleeping\n");
		pthread_mutex_unlock(&philo->data->mutex);
	}
	return (TRUE);
}

int	philo_think(t_philo *philo)
{
	if (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu milliseconds : philosopher %d is thinking\n");
		pthread_mutex_unlock(&philo->data->mutex);
	}
	else
		wait_action(philo, (philo->time_to_die - philo->time_to_eat - \
		philo->time_to_sleep));
	return (TRUE);
}

void	*philo_routine(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->die_mutex);
		pthread_mutex_lock(&philo->data->end_mutex);
		if (philo->data->died || philo->data->end)
		{
			pthread_mutex_unlock(&philo->data->die_mutex);
			pthread_mutex_unlock(&philo->data->end_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->die_mutex);
		pthread_mutex_unlock(&philo->data->end_mutex);
		if (philo_eat(philo) == FALSE)
			return (NULL);
		wait_action(philo, philo->time_to_eat);
		release_different_forks(philo);
		philo_sleep(philo);
		wait_action(philo, philo->time_to_sleep);
		philo_think(philo);
		if (philo->philo_number == 1)
			return (single_philo_dies(philo));
	}
	return (NULL);
}
