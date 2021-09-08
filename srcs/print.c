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

void    print_msg(t_philo *philo, char *msg)
{
    printf(msg, philo->data->time, philo->id);
}

int print_error(char *msg, t_philo *philo)
{
    if (philo)
        pthread_mutex_lock(&philo->data->mutex);
    ft_putstr_fd(msg, 2);
    ft_putstr_fd("\n", 2);
    if (philo)
        pthread_mutex_unlock(&philo->data->mutex);
    return (ERROR);
}
