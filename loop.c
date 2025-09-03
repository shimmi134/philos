/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:13:29 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/03 14:54:03 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	get_fork_even(t_philo *p)
{
	pthread_mutex_lock(&p->r_fork);
	pthread_mutex_lock(&p->rules->mutex);
	if (p->rules->die == 0)
		printf("%ld %i took a fork\n", get_time(), p->num);
	pthread_mutex_unlock(&p->rules->mutex);
	if (get_time() - p->time >= p->rules->death_time)
	{
		pthread_mutex_unlock(&p->r_fork);
		return ;
	}
	pthread_mutex_lock(p->l_fork);
	pthread_mutex_lock(&p->rules->mutex);
	if (get_time() - p->time >= p->rules->death_time)
	{
		pthread_mutex_unlock(&p->rules->mutex);
		pthread_mutex_unlock(&p->r_fork);
		pthread_mutex_unlock(p->l_fork);
		return ;
	}
	pthread_mutex_unlock(&p->rules->mutex);
	pthread_mutex_lock(&p->rules->mutex);
	if (p->rules->die == 0)
		printf("%ld %i took a fork\n", get_time(), p->num);
	pthread_mutex_unlock(&p->rules->mutex);
}

void	get_fork_odd(t_philo *p)
{
	pthread_mutex_lock(p->l_fork);
	pthread_mutex_lock(&p->rules->mutex);
	if (p->rules->die == 0)
		printf("%ld %i took a fork\n", get_time(), p->num);
	pthread_mutex_unlock(&p->rules->mutex);
	if (get_time() - p->time >= p->rules->death_time)
	{
		pthread_mutex_unlock(p->l_fork);
		return ;
	}
	pthread_mutex_lock(&p->r_fork);
	pthread_mutex_lock(&p->rules->mutex);
	if (get_time() - p->time >= p->rules->death_time)
	{
		pthread_mutex_unlock(&p->rules->mutex);
		pthread_mutex_unlock(&p->r_fork);
		pthread_mutex_unlock(p->l_fork);
		return ;
	}
	pthread_mutex_unlock(&p->rules->mutex);
	pthread_mutex_lock(&p->rules->mutex);
	if (p->rules->die == 0)
		printf("%ld %i took a fork\n", get_time(), p->num);
	pthread_mutex_unlock(&p->rules->mutex);
}

void	mut_cleanup(t_philo *philo)
{
	if (philo->num % 2 == 0)
	{
		pthread_mutex_unlock(&(philo->r_fork));
		pthread_mutex_unlock(philo->l_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(&(philo->r_fork));
	}
}

int	philo_eat(t_philo *philo)
{
	if (philo->rules->n_philos % 2 != 0
		&& (get_time() - philo->time)
		< (philo->rules->death_time / 3 * 2))
		usleep(300);
	if (get_time() - philo->time >= philo->rules->death_time)
		return (1);
	if (philo->num % 2 == 0)
		get_fork_even(philo);
	else
		get_fork_odd(philo);
	pthread_mutex_lock(&philo->rules->mutex);
	if (philo->died != 0 || philo->rules->die != 0)
		return (pthread_mutex_unlock(&philo->rules->mutex), 1);
	pthread_mutex_unlock(&philo->rules->mutex);
	pthread_mutex_lock(&philo->rules->mutex);
	philo->time = get_time();
	if (philo->rules->die == 0)
		printf("%ld %i is eating\n", philo->time, philo->num);
	pthread_mutex_unlock(&philo->rules->mutex);
	usleep(philo->rules->eat_time * 1000);
	philo->times_eaten += 1;
	return (mut_cleanup(philo), 0);
}

int	philo_loop(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->rules->mutex);
		if (philo->died != 0 || philo->rules->die != 0)
			return (pthread_mutex_unlock(&philo->rules->mutex), 1);
		pthread_mutex_unlock(&philo->rules->mutex);
		if (philo_eat(philo))
			return (0);
		if (philo->times_eaten == philo->rules->must_eat)
			return (printf("%ld %i is full\n", get_time(), philo->num), 0);
		philo_sleep(philo);
		pthread_mutex_lock(&philo->rules->mutex);
		if (philo->died != 0 || philo->rules->die != 0)
			return (pthread_mutex_unlock(&philo->rules->mutex), 1);
		pthread_mutex_unlock(&philo->rules->mutex);
		pthread_mutex_lock(&philo->rules->mutex);
		if (philo->died != 0 || philo->rules->die != 0)
			return (pthread_mutex_unlock(&philo->rules->mutex), 1);
		pthread_mutex_unlock(&philo->rules->mutex);
		philo_think(philo);
	}
	return (0);
}

void	*philo_main(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->rules->n_philos == 1 && philo->rules->must_eat != 0)
		return (ft_lonely_philo(philo), NULL);
	pthread_mutex_lock(&philo->rules->mutex);
	philo->time = get_time();
	pthread_mutex_unlock(&philo->rules->mutex);
	if (philo->num % 2 != 0)
		usleep(200);
	if (philo->num == philo->rules->n_philos)
		usleep(50);
	philo_loop(philo);
	return (NULL);
}
