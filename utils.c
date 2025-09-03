/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shimi-be <shimi-be@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:13:21 by shimi-be          #+#    #+#             */
/*   Updated: 2025/09/03 16:00:00 by shimi-be         ###   ########.fr       */
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
	struct timeval	tv;
	static time_t	start_time;

	if (start_time == 0)
	{
		gettimeofday(&tv, NULL);
		start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) - start_time);
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

void	print_error(char *str)
{
	printf("%s has to be a positive number smaller than max int\n", str);
}
