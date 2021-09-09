#include "philosophers.h"

int    init_structs(t_data **infos, t_philo **philo, char **args)
{
    *infos = malloc(sizeof(t_data));
    memset(*infos, 0, sizeof(t_data));
    (*infos)->forks_mutex = malloc(sizeof(pthread_mutex_t) * ft_atoi(args[1]));
    if (!((*infos)->forks_mutex))
        return (print_error("Malloc error\n", NULL));
    memset((*infos)->forks_mutex, 0, sizeof(pthread_mutex_t) * ft_atoi(args[1]));
    *philo = malloc(sizeof(t_philo) * ft_atoi(args[1]));
    if (!*philo)
        return (print_error("Malloc error\n", NULL));
    memset(*philo, 0, sizeof(t_philo) * ft_atoi(args[1]));
    return (TRUE);
}

int    shared_data(t_data *infos, char **av)
{
    infos->start_time = get_start_time();
    infos->threads = malloc(sizeof(pthread_t) * ft_atoi(av[1]));
    if (!infos->threads)
        print_error("Malloc error\n", NULL);
    return (TRUE);
}

void    init_philo(t_philo *philo, t_data *infos, unsigned long i, char **av)
{
    philo->philo_number = ft_atoi(av[1]);
    philo->time_to_die = ft_atoi(av[2]);
    philo->time_to_eat = ft_atoi(av[3]);
    philo->time_to_sleep = ft_atoi(av[4]);
    if (av[5])
        philo->nb_of_times_eat = ft_atoi(av[5]);
    philo->data = infos;
    philo->id = i + 1;
    philo->right = i + 1;
    philo->left = i;
    if (i == philo->philo_number - 1)
    {
        philo->right = i;
        philo->left = 0;
    }
}

int    create_forks_a_philo(unsigned long i, t_data *infos, t_philo *philo, char **av)
{
    while (i < (unsigned long)ft_atoi(av[1]))
    {
        memset(&philo[i], 0, sizeof(t_philo));
        init_philo(&philo[i], infos, i, av);
        if (pthread_mutex_init(&philo->data->forks_mutex[i], NULL))
            return (print_error("Error in attempt to init mutex\n", philo));
        i++;
    }
    if (pthread_mutex_init(&philo->data->mutex, NULL))
        return (print_error("Error in attempt to init mutex\n", philo));
    if (pthread_mutex_init(&philo->data->die_mutex, NULL))
        return (print_error("Error in attempt to init mutex\n", philo));
    if (pthread_mutex_init(&philo->data->time_mutex, NULL))
        return (print_error("Error in attempt to init mutex\n", philo));
    if (pthread_mutex_init(&philo->data->count_mutex, NULL))
        return (print_error("Error in attempt to init mutex\n", philo));
    if (pthread_mutex_init(&philo->data->lm_mutex, NULL))
        return (print_error("Error in attempt to init mutex\n", philo));
    start_threads(philo, philo->philo_number);
    return (TRUE);
}
