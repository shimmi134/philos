/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tsd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:13:13 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/03 15:17:26 by shimi-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	philo_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->mutex);
	printf(RED "%ld %i died\n" RESET, get_time(), philo->num);
	philo->rules->die = 1;
	pthread_mutex_unlock(&philo->rules->mutex);
}

void	ft_lonely_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->mutex);
	printf(GREEN "%ld %i has taken a fork\n" RESET, get_time(), philo->num);
	usleep(philo->rules->death_time * 1000);
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
	pthread_mutex_unlock(&philo->rules->mutex);
	pthread_mutex_lock(&philo->rules->mutex);
	if (philo->rules->die == 0)
		printf(CYAN "%ld %d is thinking\n" RESET, get_time(), philo->num);
	pthread_mutex_unlock(&philo->rules->mutex);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->mutex);
	if (philo->rules->die == 0)
		printf(BLUE "%ld %i is sleeping\n" RESET, get_time(), philo->num);
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
