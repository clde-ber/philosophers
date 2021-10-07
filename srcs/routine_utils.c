/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:23 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/07 17:54:09 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	single_philo_dies(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->die_mutex);
	philo->data->died = 1;
	pthread_mutex_unlock(&philo->data->die_mutex);
	return (quit_routine(philo));
}

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
