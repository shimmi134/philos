/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:54:18 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/03 15:55:07 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

t_philo	**create_philos(t_rules *rules)
{
	t_philo	**philos;
	int		i;

	i = 0;
	philos = malloc(sizeof(t_philo *) * rules->n_philos);
	if (!philos)
		return (NULL);
	while (i < rules->n_philos)
	{
		philos[i] = malloc(sizeof(t_philo));
		(philos[i])->num = i + 1;
		(philos[i])->times_eaten = 0;
		(philos[i])->time = 0;
		(philos[i])->rules = rules;
		(philos[i])->died = 0;
		pthread_mutex_init(&((philos[i])->r_fork), NULL);
		i++;
	}
	give_forks(philos, rules->n_philos);
	return (philos);
}

t_rules	*create_rules(char **av)
{
	t_rules	*rules;

	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (NULL);
	rules->n_philos = ft_atoi(av[1]);
	rules->death_time = ft_atoi(av[2]);
	rules->eat_time = ft_atoi(av[3]);
	rules->sleep_time = ft_atoi(av[4]);
	if (av[5])
		rules->must_eat = ft_atoi(av[5]);
	else
		rules->must_eat = -1;
	rules->die = 0;
	pthread_mutex_init(&(rules->mutex), NULL);
	return (rules);
}

void	create_monitor(t_rules *rules)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, monitoring, rules);
	pthread_join(monitor, NULL);
}

void	destroy_philos(t_philo **philos, int philos_num)
{
	int	i;

	i = 0;
	while (i < philos_num)
	{
		pthread_join((philos[i])->thread, NULL);
		free(philos[i]);
		i++;
	}
	free(philos);
}

void	give_forks(t_philo **p, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		if (i == 0)
			(p[i])->l_fork = &((p[num - 1])->r_fork);
		else
			(p[i])->l_fork = &((p[i - 1])->r_fork);
		pthread_create(&(p[i])->thread, NULL, philo_main, p[i]);
		i++;
	}
}
