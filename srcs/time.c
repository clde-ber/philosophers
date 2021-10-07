/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:39 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/07 18:05:43 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	get_time(t_philo *philo)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (print_error("Error in getting time\n", philo));
	return ((tv.tv_sec * 1000000 + tv.tv_usec) - philo->data->start_time);
}

unsigned long	get_timestamp(t_philo *philo)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (print_error("Error in getting time\n", philo));
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

unsigned long	get_start_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (print_error("Error in getting time\n", NULL));
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void	wait_action(t_philo *philo, unsigned long time)
{
	unsigned long	time1;
	unsigned long	time2;	

	philo->diff = 0;
	pthread_mutex_lock(&philo->data->time_cmp_mutex);
	time1 = get_timestamp(philo);
	pthread_mutex_unlock(&philo->data->time_cmp_mutex);
	pthread_mutex_lock(&philo->data->time_mutex);
	time2 = get_timestamp(philo);
	pthread_mutex_unlock(&philo->data->time_mutex);
	while (philo->diff < time)
	{
		pthread_mutex_lock(&philo->data->time_mutex);
		time2 = get_timestamp(philo);
		pthread_mutex_unlock(&philo->data->time_mutex);
		philo->diff = time2 - time1;
		usleep(1000);
	}
}
