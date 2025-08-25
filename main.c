#include "philos.h"

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


int	philo_eat(t_philo *philo)
{
	long time;


	pthread_mutex_lock(&(philo->r_fork));

	pthread_mutex_lock(philo->l_fork);
	pthread_mutex_lock(&(philo->rules->mutex));
	philo->time = get_time();
	printf("%ld %i has eaten\n", philo->time, philo->num);
	philo->times_eaten++;
	pthread_mutex_unlock(&(philo->rules->mutex));
	usleep(philo->sleep_time*1000);
	pthread_mutex_unlock(&(philo->r_fork));
	pthread_mutex_unlock(philo->l_fork);
	return (0);
}

void	ft_lonely_philo(t_philo *p)
{
	pthread_mutex_lock(p->rules->mutex);
	printf("%ld %i has taken a fork\n", get_time(), philo->num);
	usleep(p->death_time*1000);
	printf("%ld %i died\n", get_time(), p->num);
	pthread_mutex_unlock(p->rules->mutex);

}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(philo->rules->mutex);
	if (get_time() - philo->time >= philo->rules->death_time)
		return (philo_death(philo));
	if (philo->rules->die != 0)
		printf("%ld %d is thinking\n", gettime(), philo->num);
	pthread_mutex_unlock(philo->rules->mutex);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(philo->rules->mutex);
	if (philo->rules->die != 0)
		printf("%ld %i is sleeping\n", get_time(), philos->num);
	usleep(philos->rules->sleep_time * 1000);
	pthread_mutex_unlock(philo->rules->mutex);
}

void	*philo_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	if (philo->rules->n_philos == 1)
		return (ft_lonely_philo(p), NULL);
	philo->time = get_time();
	pthread_mutex_lock(philo->rules->mutex);
	while (philo->rules->die == 0 && philo->times_eaten != philo->rules->must_eat)
	{
		pthread_mutex_unlock(philo->rules->mutex);
		pthread_mutex_lock(philo->rules->mutex);
		if (philo->died != 0|| philo->rules->die != 0)
		{
			if (philo->died)
				philo_death(philo);
			break;
		}
		pthread_mutex_unlock(philo->rules->mutex);
		philo_eat(philo);
		pthread_mutex_lock(philo->rules->mutex);
		if (philo->times_eaten == philo->rules->must_eat)
		{
			philo_is_full(philo);
			break ;
		}
		if (philo->died != 0|| philo->rules->die != 0)
		{
			if (philo->died)
				philo_death(philo);
			break;
		}
		pthread_mutex_unlock(philo->rules->mutex);
		philo_sleep(philo);
		pthread_mutex_lock(philo->rules->mutex);
		if (philo->died != 0|| philo->rules->die != 0)
		{
			if (philo->died)
				philo_death(philo);
			break;
		}
		pthread_mutex_unlock(philo->rules->mutex);
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

t_rules *create_rules(int ac, char **av)
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

void	destroy_philos(t_philo **philos, int j)
{
	int		i;

	i = 0;
	while (i < j)
	{
		pthread_join((philos[i])->thread, NULL);
		free(philos[i]);
		i++;
	}
	free(philos);
}

t_philo** create_philos(t_rules *rules)
{
	t_philo **philos;

	pthread_mutex_lock(&(rules->mutex));
	philos = malloc(sizeof(t_philo *) * rules->n_philos);
	if (!philos)
		return (NULL);
	for (int i = 0; i < rules->n_philos; i++){	
		philos[i] = malloc(sizeof(t_philo*));
		(philos[i])->num = i+1;
		(philos[i])->times_eaten = 0;
		(philos[i])->time = 0;//gettime();
		(philos[i])->rules = rules;
		(philos[i])->died = 0;
		pthread_mutex_init(&((philos[i])->r_fork), NULL);
	}
	give_forks(philos, rules->n_philos);
	pthread_mutex_unlock(&(rules->mutex));
	return (philos);
}

int main(int ac, char *av[])
{
	pthread_t	*t;
	t_rules		*rules;
	t_philo		**philos;

	if (ac < 4 || ac > 6)
	{
		printf("Wrong argument number, only 4 or 5!\n");
		return (1);
	}
	rules = create_rules(ac, av);
	if (!rules)
	{
		printf("Error creating rules, try again.\n");
		return (2);
	}
	philos = create_philos(rules);
	destroy_philos(philos, rules->n_philos);
	free(rules);
	return (0);
}
