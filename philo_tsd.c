#include "philos.h"

void	philo_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->mutex);
	printf("%ld %i died\n", get_time(), philo->num);
	philo->rules->die = 1;
	pthread_mutex_unlock(&philo->rules->mutex);
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
	{
		pthread_mutex_unlock(&philo->rules->mutex);
		return ;
	}
	if (philo->rules->die != 0)
		printf("%ld %d is thinking\n", get_time(), philo->num);
	pthread_mutex_unlock(&philo->rules->mutex);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->mutex);
	if (philo->rules->die == 0)
		printf("%ld %i is sleeping\n", get_time(), philo->num);
	pthread_mutex_unlock(&philo->rules->mutex);
	usleep(philo->rules->sleep_time * 1000);
	pthread_mutex_lock(&philo->rules->mutex);
	if (get_time() - philo->time >= philo->rules->death_time)
	{
		pthread_mutex_unlock(&philo->rules->mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->rules->mutex);
}
