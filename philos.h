#ifndef PHILOS_H
#define PHILOS_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

typedef struct s_philo t_philo;

typedef struct s_rule
{
	int				die;
	int				n_philos;
	int				eat_time;
	int				must_eat;
	int				death_time;
	int				sleep_time;
	int				start_time;
	t_philo			**philos;
	pthread_mutex_t	mutex;
}		t_rules;

typedef struct s_philo
{
	int				num;
	int				times_eaten;
	int				died;
	long			time;
	t_rules			*rules;
	pthread_t		thread;
	pthread_mutex_t *l_fork;	//fork you steal from the one on your left
	pthread_mutex_t r_fork;		//Your own fork
}		t_philo;


// FUNCTIONS
int			ft_atoi(const char *nptr);
int			philo_eat(t_philo *philo);
int			ft_isdigit(int c);
int			ft_isstr_num(char *str);
int			parse_input(char **av);
long		get_time();
void		get_fork_even(t_philo *p);
void		get_fork_odd(t_philo *p);
void		philo_death(t_philo *philo);
void		ft_lonely_philo(t_philo *philo);
void		philo_think(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		give_forks(t_philo **p, int num);
void		destroy_philos(t_philo **philos, int philos_num);
void		*philo_main(void *arg);
t_rules		*create_rules(char **av);
t_philo		**create_philos(t_rules *rules);

#endif
