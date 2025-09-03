/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:13:29 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/03 15:43:11 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

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
			return (printf(YELLOW "%ld %i is full\n" RESET, get_time(),
					philo->num), 0);
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
