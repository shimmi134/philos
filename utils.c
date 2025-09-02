
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

long	get_time(void)
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
	if ((ft_atoi(av[1]) < 0 || ft_atoi(av[1]) >= INT_MAX) || !ft_isstr_num(av[1]))
		return (printf("The number of philosophers has to be a positive number smaller than int max.\n"),1);
	if (ft_atoi(av[2]) < 0 || ft_atoi(av[2]) >= INT_MAX || !ft_isstr_num(av[2]))
		return(printf("Death time has to be a positive number smaller than int max.\n"),1);
	if (ft_atoi(av[3]) < 0 || ft_atoi(av[3]) >= INT_MAX || !ft_isstr_num(av[3]))
		return(printf("Eating time has to be a positive number smaller than int max.\n"),1);
	if (ft_atoi(av[4]) < 0 || ft_atoi(av[4]) >= INT_MAX || !ft_isstr_num(av[4]))
		return(printf("Sleeping time has to be a positive number smaller than int max.\n"),1);
	if (av[5] && (ft_atoi(av[5]) < 0 || ft_atoi(av[5]) >= INT_MAX || !ft_isstr_num(av[5])))
		return(printf("Times each philo must eat has to be a positive number smaller than int max.\n"),1);
	return (0);
}
