#include "philosophers.h"

int destroy_mutexes(unsigned long i, t_philo *philo)
{
    if (pthread_mutex_destroy(&philo->data->mutex))
        return (print_error("Error in attempt to destroy mutex\n", philo));
    while (i < philo->data->philo_number)
    {
        if (pthread_mutex_destroy(&philo->data->forks_mutex[i]))
            return (print_error("Error in attempt to destroy mutex\n", philo));
        i++;
    }
    return (TRUE);
}

void    take_different_forks(t_philo *philo)
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


void    release_different_forks(t_philo *philo)
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