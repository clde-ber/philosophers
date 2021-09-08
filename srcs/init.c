#include "philosophers.h"

int    init_structs(t_data **infos, t_philo **philo, char **args)
{
    *infos = malloc(sizeof(t_data));
    memset(*infos, 0, sizeof(t_data));
    (*infos)->forks_mutex = malloc(sizeof(pthread_mutex_t) * ft_atoi(args[1]));
    if (!((*infos)->forks_mutex))
        return (print_error("Malloc error\n"));
    memset((*infos)->forks_mutex, 0, sizeof(pthread_mutex_t) * ft_atoi(args[1]));
    *philo = malloc(sizeof(t_philo) * ft_atoi(args[1]));
    if (!*philo)
        return (print_error("Malloc error\n"));
    memset(*philo, 0, sizeof(t_philo) * ft_atoi(args[1]));
    return (TRUE);
}

int    shared_data(t_data *infos, char **av)
{
    infos->philo_number = ft_atoi(av[1]);
    infos->time_to_die = ft_atoi(av[2]);
    infos->time_to_eat = ft_atoi(av[3]);
    infos->time_to_sleep = ft_atoi(av[4]);
    infos->time_to_think = infos->time_to_die - infos->time_to_eat - infos->time_to_sleep;
    if (av[5])
        infos->nb_of_times_eat = ft_atoi(av[5]);
    infos->start_time = get_start_time();
    infos->threads = malloc(sizeof(pthread_t) * infos->philo_number);
    if (!infos->threads)
        print_error("Malloc error\n");
    return (TRUE);
}

void    init_philo(t_philo *philo, t_data *infos, unsigned long i)
{
    philo->data = infos;
    philo->id = i + 1;
    philo->eat_count = 0;
    philo->right = i + 1;
    philo->left = i;
    if (i == philo->data->philo_number - 1)
    {
        philo->right = i;
        philo->left = 0;
    }
}

int    create_forks_a_philo(unsigned long i, t_data *infos, t_philo *philo)
{
    while (i < infos->philo_number)
    {
        memset(&philo[i], 0, sizeof(t_philo));
        init_philo(&philo[i], infos, i);
        if (pthread_mutex_init(&philo->data->forks_mutex[i], NULL))
            return (print_error("Error in attempt to init mutex\n"));
        i++;
    }
    if (pthread_mutex_init(&philo->data->mutex, NULL))
        return (print_error("Error in attempt to init mutex\n"));
    start_threads(philo, philo->data->philo_number);
    if (pthread_mutex_destroy(&philo->data->mutex))
        return (print_error("Error in attempt to destroy mutex\n"));
    i = 0;
    while (i < infos->philo_number)
    {
        if (pthread_mutex_destroy(&philo->data->forks_mutex[i]))
            return (print_error("Error in attempt to destroy mutex\n"));
        i++;
    }
    return (TRUE);
}
