#include "philosophers.h"

int    philo_eat(t_philo *philo)
{
    if (get_time(philo) > philo->last_meal + philo->data->time_to_die)
    {
        pthread_mutex_lock(&philo->data->mutex);
        print_msg(philo, "%lu milliseconds : philosopher %lu died\n");
        pthread_mutex_unlock(&philo->data->mutex);
        philo->data->died = 1;
        destroy_mutexes(0, philo);
    }
    take_different_forks(philo);
    pthread_mutex_lock(&philo->data->mutex);
    print_msg(philo, "%lu milliseconds : philosopher %lu has taken a fork\n");
    print_msg(philo, "%lu milliseconds : philosopher %lu is eating\n");
    philo->last_meal = get_time(philo);
    philo->eat_count++;
    pthread_mutex_unlock(&philo->data->mutex);
    return (TRUE);
}

int    philo_sleep(t_philo *philo)
{
    (void)time;
    pthread_mutex_lock(&philo->data->mutex);
    print_msg(philo, "%lu milliseconds : philosopher %lu is sleeping\n");
    pthread_mutex_unlock(&philo->data->mutex);
    return (TRUE);
}

int    philo_think(t_philo *philo)
{
    (void)time;
    pthread_mutex_lock(&philo->data->mutex);
    print_msg(philo, "%lu milliseconds : philosopher %lu is thinking\n");
    pthread_mutex_unlock(&philo->data->mutex);
    return (TRUE);
}

void    *philo_routine(t_philo *philo)
{
    while (!philo->data->died || philo->eat_count < philo->data->nb_of_times_eat)
    {
        philo_eat(philo);
        wait_action(philo, philo->data->time_to_eat);
        release_different_forks(philo);
        philo_sleep(philo);
        wait_action(philo, philo->data->time_to_sleep);
        philo_think(philo);
    }
    return (NULL);
}

void    *start_routine(void *philo)
{
   t_philo *phil;

    phil = (t_philo *)philo;
    pthread_mutex_lock(&phil->data->mutex);
    print_msg(philo, "%lu milliseconds : philosopher %lu is alive\n");
    pthread_mutex_unlock(&phil->data->mutex);
    philo_routine(philo);
    return (TRUE);
}
