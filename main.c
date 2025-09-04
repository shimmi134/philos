/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:13:42 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/04 13:38:03 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	parse_input(char **av)
{
	if ((ft_atoi(av[1]) <= 0 || ft_atoi(av[1]) >= INT_MAX)
		|| !ft_isstr_num(av[1]))
		return (print_error("The number of philosophers"), 1);
	if (ft_atoi(av[2]) <= 0 || ft_atoi(av[2]) >= INT_MAX
		|| !ft_isstr_num(av[2]))
		return (print_error("Death time"), 1);
	if (ft_atoi(av[3]) <= 0 || ft_atoi(av[3]) >= INT_MAX
		|| !ft_isstr_num(av[3]))
		return (print_error("Eating time"), 1);
	if (ft_atoi(av[4]) <= 0 || ft_atoi(av[4]) >= INT_MAX
		|| !ft_isstr_num(av[4]))
		return (print_error("Sleeping time"), 1);
	if (av[5] && (ft_atoi(av[5]) <= 0 || ft_atoi(av[5]) >= INT_MAX
			|| !ft_isstr_num(av[5])))
		return (print_error("Times each philo must eat"), 1);
	return (0);
}

int	monitoring_loop(t_rules *rules)
{
	int	eat;
	int	i;

	eat = 0;
	i = 0;
	while (i < rules->n_philos)
	{
		pthread_mutex_lock(&rules->mutex);
		if (rules->die == 0 && get_time()
			- rules->philos[i]->time >= rules->death_time
			&& rules->philos[i]->times_eaten != rules->must_eat)
		{
			pthread_mutex_unlock(&rules->mutex);
			philo_death(rules->philos[i]);
			return (1);
		}
		if (rules->philos[i]->times_eaten == rules->must_eat)
			eat++;
		if (eat == rules->n_philos)
			return (pthread_mutex_unlock(&rules->mutex), 1);
		pthread_mutex_unlock(&rules->mutex);
		i++;
	}
	return (0);
}

void	*monitoring(void *val)
{
	t_rules	*rules;

	rules = (t_rules *)val;
	while (1)
	{
		if (monitoring_loop(rules))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

int	main(int ac, char *av[])
{
	t_rules	*rules;
	t_philo	**philos;

	if (ac < 5 || ac > 6)
	{
		printf("Wrong argument number, only 4 or 5!\n");
		return (1);
	}
	if (parse_input(av))
		return (2);
	rules = create_rules(av);
	if (!rules)
	{
		printf("Error creating rules, try again.\n");
		return (3);
	}
	philos = create_philos(rules);
	rules->philos = philos;
	create_monitor(rules);
	pthread_join(rules->monitor, NULL);
	destroy_philos(philos, rules->n_philos);
	pthread_mutex_destroy(&rules->mutex);
	free(rules);
	return (0);
}
