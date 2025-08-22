#ifndef PHILOS_H
#define PHILOS_H

#include <pthread.h>

typedef struct s_philo
{
	int				num;
	int				times_eaten;
	long			time;
	t_rules			*rules;
	pthread_mutex_t *l_fork;	//fork you steal from the one on your left
	pthread_mutex_t r_fork;		//Your own fork
	

}		t_philo;

typedef struct s_rule
{
	int eat_time;
	int	death_time;
	int	sleep_time;
	int must_eat;
	int start_time;
	int n_philos;
	pthread_mutex_t mutex;
}	t_rules;


#endif
