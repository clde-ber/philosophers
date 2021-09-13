/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:48 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/13 09:59:20 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	quit_routine(t_philo *philo)
{
	if ((philo->eat_count < philo->nb_of_times_eat && philo->nb_of_times_eat))
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu milliseconds : philosopher %lu died\n");
		pthread_mutex_unlock(&philo->data->mutex);
	}
	if ((philo->eat_count >= philo->nb_of_times_eat && philo->nb_of_times_eat) \
	|| philo->nb_of_times_eat == 0)
	{
		pthread_mutex_lock(&philo->data->end_mutex);
		philo->data->end = 1;
		pthread_mutex_unlock(&philo->data->end_mutex);
	}
	else
	{
		pthread_mutex_lock(&philo->data->die_mutex);
		philo->data->died = 1;
		pthread_mutex_unlock(&philo->data->die_mutex);
	}
	return (FALSE);
}

int	philo_eat(t_philo *philo)
{
	if (get_time(philo) > philo->last_meal + philo->time_to_die * 1000 || \
	(philo->eat_count >= philo->nb_of_times_eat && philo->nb_of_times_eat) || \
	philo->nb_of_times_eat == 0)
		return (quit_routine(philo));
	take_different_forks(philo);
	pthread_mutex_lock(&philo->data->lm_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->data->lm_mutex);
	pthread_mutex_lock(&philo->data->mutex);
	print_msg(philo, "%lu milliseconds : philosopher %lu has taken a fork\n");
	print_msg(philo, "%lu milliseconds : philosopher %lu is eating\n");
	pthread_mutex_unlock(&philo->data->mutex);
	pthread_mutex_lock(&philo->data->count_mutex);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->count_mutex);
	return (TRUE);
}

int	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex);
	print_msg(philo, "%lu milliseconds : philosopher %lu is sleeping\n");
	pthread_mutex_unlock(&philo->data->mutex);
	return (TRUE);
}

int	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex);
	print_msg(philo, "%lu milliseconds : philosopher %lu is thinking\n");
	pthread_mutex_unlock(&philo->data->mutex);
	return (TRUE);
}

void	*philo_routine(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->die_mutex);
		pthread_mutex_lock(&philo->data->count_mutex);
		pthread_mutex_lock(&philo->data->end_mutex);
		if (philo->data->died || philo->data->end)
		{
			pthread_mutex_unlock(&philo->data->die_mutex);
			pthread_mutex_unlock(&philo->data->count_mutex);
			pthread_mutex_unlock(&philo->data->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->die_mutex);
		pthread_mutex_unlock(&philo->data->count_mutex);
		pthread_mutex_unlock(&philo->data->end_mutex);
		if (philo_eat(philo) == FALSE)
			break ;
		wait_action(philo, philo->time_to_eat * 1000);
		release_different_forks(philo);
		philo_sleep(philo);
		wait_action(philo, philo->time_to_sleep * 1000);
		philo_think(philo);
	}
	return (NULL);
}
