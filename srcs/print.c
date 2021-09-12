/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:42 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/12 08:53:49 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s || !fd)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	print_msg(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->die_mutex);
	pthread_mutex_lock(&philo->data->end_mutex);
	if ((!philo->data->died && !philo->data->end) || \
	philo->nb_of_times_eat == 0)
	{
		printf(msg, get_time(philo) / 1000, philo->id);
	}
	pthread_mutex_unlock(&philo->data->die_mutex);
	pthread_mutex_unlock(&philo->data->end_mutex);
}

int	print_error(char *msg, t_philo *philo)
{
	if (philo)
		pthread_mutex_lock(&philo->data->mutex);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	if (philo)
		pthread_mutex_unlock(&philo->data->mutex);
	return (ERROR);
}
