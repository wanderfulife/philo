/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:45 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:03:47 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}

static bool	parse_arguments(t_program *prog, int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_positive_int(argv[i]))
			return (false);
		i++;
	}
	prog->philo_count = ft_atoi(argv[1]);
	prog->time_to_die = ft_atoi(argv[2]);
	prog->time_to_eat = ft_atoi(argv[3]);
	prog->time_to_sleep = ft_atoi(argv[4]);
	prog->must_eat_count = -1;
	if (argc == 6)
		prog->must_eat_count = ft_atoi(argv[5]);
	if (prog->philo_count <= 0 || prog->time_to_die <= 0
		|| prog->time_to_eat <= 0 || prog->time_to_sleep <= 0)
		return (false);
	if (argc == 6 && prog->must_eat_count <= 0)
		return (false);
	return (true);
}

static bool	init_mutexes(t_program *program)
{
	int	i;

	program->forks = malloc(sizeof(pthread_mutex_t) * program->philo_count);
	if (!program->forks)
		return (false);
	i = 0;
	while (i < program->philo_count)
	{
		if (pthread_mutex_init(&program->forks[i], NULL) != 0)
			return (false);
		i++;
	}
	if (pthread_mutex_init(&program->death_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&program->print_mutex, NULL) != 0)
		return (false);
	return (true);
}

static bool	init_philosophers(t_program *program)
{
	int	i;

	program->philos = malloc(sizeof(t_philo) * program->philo_count);
	if (!program->philos)
		return (false);
	i = 0;
	while (i < program->philo_count)
	{
		program->philos[i].id = i + 1;
		program->philos[i].meals_eaten = 0;
		program->philos[i].last_meal_time = get_time();
		program->philos[i].prog = program;
		program->philos[i].left_fork = &program->forks[i];
		program->philos[i].right_fork = &program->forks[(i + 1)
			% program->philo_count];
		i++;
	}
	return (true);
}

bool	init_program(t_program *prog, int argc, char **argv)
{
	prog->someone_died = false;
	prog->start_time = get_time();
	if (!parse_arguments(prog, argc, argv))
		return (false);
	if (!init_mutexes(prog))
		return (false);
	if (!init_philosophers(prog))
		return (false);
	return (true);
}
