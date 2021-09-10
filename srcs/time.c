/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:58 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/10 16:41:03 by user42           ###   ########.fr       */
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

unsigned long	get_start_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (print_error("Error in getting time\n", NULL));
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void	wait_action(t_philo *philo, unsigned long time)
{
	unsigned long	cumul_time;
	unsigned long	diff;

	cumul_time = get_time(philo);
	if (philo->time_cmp)
		diff = cumul_time % philo->time_cmp;
	else
		diff = 0;
	while (1)
	{
		if (cumul_time >= philo->time_cmp + time - diff)
			break ;
		cumul_time = get_time(philo);
		if (philo->time_cmp)
			diff = cumul_time % philo->time_cmp;
		else
			diff = 0;
		usleep(1);
	}
	philo->time_cmp += time;
}
