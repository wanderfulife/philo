/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:04:07 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:50:18 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	msleep(long long ms)
{
	long long	start;
	long long	current;

	start = get_time();
	while (1)
	{
		current = get_time();
		if (current - start >= ms)
			break ;
		usleep(500);
	}
}

void	log_status(t_philo *philo, char *status)
{
	long long	timestamp;
	bool		should_print;

	pthread_mutex_lock(&philo->prog->death_lock);
	should_print = !philo->prog->has_death;
	if (should_print)
		timestamp = get_time() - philo->prog->start_ms;
	pthread_mutex_unlock(&philo->prog->death_lock);
	if (should_print)
	{
		pthread_mutex_lock(&philo->prog->print_lock);
		print_colored_status(philo, status, timestamp);
		pthread_mutex_unlock(&philo->prog->print_lock);
	}
}

static bool	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

bool	is_valid_num(char *str)
{
	long long	num;
	int			i;

	if (!is_number(str))
		return (false);
	num = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		if (num > INT_MAX)
			return (false);
		i++;
	}
	return (num > 0);
}
