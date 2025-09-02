/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:54:35 by shimi-be          #+#    #+#             */
/*   Updated: 2025/08/26 16:54:37 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <sys/time.h>

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	result = 0;
	sign = 1;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

long	get_time()
{
	struct timeval tv;
	static time_t start_time;

	if (start_time == 0)
	{
		gettimeofday(&tv, NULL);
		start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec/1000) - start_time);
}

void	philo_death(t_philo *philo)
{
	printf("%ld %i died\n", get_time(), philo->num);
}


int	philo_eat(t_philo *philo)
{
	if (get_time() - philo->time >= philo->rules->death_time)
		return (philo_death(philo), 1);
	pthread_mutex_lock(&(philo->r_fork));	
	printf("%ld %i has taken a fork\n", get_time(), philo->num);
	if (get_time() - philo->time >= philo->rules->death_time)
		return (philo_death(philo), 1);
	pthread_mutex_lock(philo->l_fork);
	printf("%ld %i has taken a fork\n", get_time(), philo->num);
	usleep(philo->rules->eat_time*1000);
	philo->time = get_time();
	printf("%ld %i has eaten\n", philo->time, philo->num);
	pthread_mutex_unlock(&(philo->r_fork));
	pthread_mutex_unlock(philo->l_fork);
	philo->times_eaten++;
	return (0);
}

void	ft_lonely_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->mutex);
	printf("%ld %i has taken a fork\n", get_time(), philo->num);
	usleep(philo->rules->death_time*1000);
	printf("%ld %i died\n", get_time(), philo->num);
	pthread_mutex_unlock(&philo->rules->mutex);

}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->mutex);
	if (get_time() - philo->time >= philo->rules->death_time)
		return (philo_death(philo));
	if (philo->rules->die != 0)
		printf("%ld %d is thinking\n", get_time(), philo->num);
	pthread_mutex_unlock(&philo->rules->mutex);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->mutex);
	if (philo->rules->die != 0)
		printf("%ld %i is sleeping\n", get_time(), philo->num);
	usleep(philo->rules->sleep_time * 1000);
	pthread_mutex_unlock(&philo->rules->mutex);
}

void	*philo_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->num % 2 != 0)
		usleep(50);
	if (philo->rules->n_philos == 1)
		return (ft_lonely_philo(philo), NULL);
	philo->time = get_time();
	while (philo->rules->die == 0 && philo->times_eaten != philo->rules->must_eat)
	{
		pthread_mutex_lock(&philo->rules->mutex);
		if (philo->died != 0|| philo->rules->die != 0)
		{
			if (philo->died)
				philo_death(philo);
			pthread_mutex_unlock(&philo->rules->mutex);
			break;
		}
		pthread_mutex_unlock(&philo->rules->mutex);
		philo_eat(philo);
		pthread_mutex_lock(&philo->rules->mutex);
		if (philo->times_eaten == philo->rules->must_eat)
		{
			//philo_is_full(philo);
			break ;
		}
		if (philo->died != 0|| philo->rules->die != 0)
		{
			if (philo->died)
				philo_death(philo);
			pthread_mutex_unlock(&philo->rules->mutex);
			break;
		}
		pthread_mutex_unlock(&philo->rules->mutex);
		philo_sleep(philo);
		pthread_mutex_lock(&philo->rules->mutex);
		if (philo->died != 0|| philo->rules->die != 0)
		{
			if (philo->died)
				philo_death(philo);
			pthread_mutex_unlock(&philo->rules->mutex);
			break;
		}
		pthread_mutex_unlock(&philo->rules->mutex);
		philo_think(philo);
	}

	return (NULL);
}

void give_forks(t_philo **p, int num)
{
	int i = 0;
	for (; i < num; ){
		if (i == 0)
			(p[i])->l_fork = &((p[num-1])->r_fork);
		else
			(p[i])->l_fork = &((p[i-1])->r_fork);
		pthread_create(&(p[i])->thread, NULL, philo_loop,p[i]);
		i++;
	}
}

t_philo** create_philos(t_rules *rules)
{
	t_philo **philos;

	pthread_mutex_lock(&(rules->mutex));
	philos = malloc(sizeof(t_philo *) * rules->n_philos);
	if (!philos)
		return (NULL);
	for (int i = 0; i < rules->n_philos; i++){	
		philos[i] = malloc(sizeof(t_philo));
		(philos[i])->num = i+1;
		(philos[i])->times_eaten = 0;
		(philos[i])->time = 0;//gettime();
		(philos[i])->rules = rules;
		(philos[i])->died = 0;
		pthread_mutex_init(&((philos[i])->r_fork), NULL);
	}
	printf("Gothere\n");
	give_forks(philos, rules->n_philos);
	pthread_mutex_unlock(&(rules->mutex));
	return (philos);
}

void	destroy_philos(t_philo **philos, int philos_num)
{
	int		i;

	i = 0;
	while (i < philos_num)
	{
		pthread_join((philos[i])->thread, NULL);
		free(philos[i]);
		i++;
	}
	free(philos);
}

t_rules *create_rules(char **av)
{
	t_rules *rules;

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
	pthread_mutex_init(&(rules->mutex),NULL);
	return (rules);
}

int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

int	ft_isstr_num(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	parse_input(char **av)
{
	if ((ft_atoi(av[1]) <= 0 || ft_atoi(av[1]) > INT_MAX) || !ft_isstr_num(av[1]))
	{
		printf("The number of philosophers has to be a positive number smaller than int max.\n");
		return (1);
	}
	if (ft_atoi(av[2]) <= 0 || ft_atoi(av[2]) > INT_MAX || !ft_isstr_num(av[2]))
	{
		printf("Death time has to be a positive number smaller than int max.\n");
		return (1);
	}
	if (ft_atoi(av[3]) <= 0 || ft_atoi(av[3]) > INT_MAX || !ft_isstr_num(av[3]))
	{
		printf("Eating time has to be a positive number smaller than int max.\n");
		return (1);
	}
	if (ft_atoi(av[4]) <= 0 || ft_atoi(av[4]) > INT_MAX || !ft_isstr_num(av[4]))
	{
		printf("Sleeping time has to be a positive number smaller than int max.\n");
		return (1);
	}
	if (av[5] && (ft_atoi(av[1]) <= 0 || ft_atoi(av[1]) > INT_MAX || !ft_isstr_num(av[5])))
	{
		printf("Times each philo must eat has to be a positive number smaller than int max.\n");
		return (1);
	}
	return (0);
}

int main(int ac, char *av[])
{
	t_rules		*rules;
	t_philo		**philos;

	if (ac < 4 || ac > 6)
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
	destroy_philos(philos, rules->n_philos);
	free(rules);
	return (0);
}
