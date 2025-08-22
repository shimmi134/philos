#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void give_forks(t_philos **p, int num)
{
	int i = 0
	for (; i < num; ){
		if (i == 0)
		{
			(p[i])->l_fork = &((p[num-1])->r_fork);
		}
		else
			(p[i])->l_fork = &((p[i-1])->r_fork);
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
	pthread_mutex_init(&(rules->mutex),NULL);
	return (rules);
}

t_philo** create_philos(t_rules *rules)
{
	t_philo **philos;

	philos = malloc(sizeof(t_philo *) * rules->n_philos);
	if (!philos)
		return (NULL);
	pthread_mutex_lock(&(rules->mutex));
	for (int i = 0; i < rules->n_philos, i++){	
		philos[i] = malloc(sizeof(t_philo));
		(philos[i])->num = i+1;
		(philos[i])->times_eaten = 0;
		(philos[i])->time = gettime();
		(philos[i])->rules = rules;
		pthread_mutex_init(&((philos[i])->r_fork), NULL);
	}
	give_forks(philos, rules->n_philos);
	pthread_mutex_unlock(&(rules->mutex));

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

}
