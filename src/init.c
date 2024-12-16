/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:45 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:52:31 by JoWander         ###   ########.fr       */
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

static bool	parse_arguments(t_sim *prog, int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_num(argv[i]))
			return (false);
		i++;
	}
	prog->num_philos = ft_atoi(argv[1]);
	prog->die_ms = ft_atoi(argv[2]);
	prog->eat_ms = ft_atoi(argv[3]);
	prog->sleep_ms = ft_atoi(argv[4]);
	prog->meals_limit = -1;
	if (argc == 6)
		prog->meals_limit = ft_atoi(argv[5]);
	if (prog->num_philos <= 0 || prog->die_ms <= 0
		|| prog->eat_ms <= 0 || prog->sleep_ms <= 0)
		return (false);
	if (argc == 6 && prog->meals_limit <= 0)
		return (false);
	return (true);
}

static bool	init_mutexes(t_sim *program)
{
	int	i;

	program->forks = malloc(sizeof(pthread_mutex_t) * program->num_philos);
	if (!program->forks)
		return (false);
	i = 0;
	while (i < program->num_philos)
	{
		if (pthread_mutex_init(&program->forks[i], NULL) != 0)
			return (false);
		i++;
	}
	if (pthread_mutex_init(&program->death_lock, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&program->print_lock, NULL) != 0)
		return (false);
	return (true);
}

static bool	init_philosophers(t_sim *program)
{
	int	i;

	program->philos = malloc(sizeof(t_philo) * program->num_philos);
	if (!program->philos)
		return (false);
	i = 0;
	while (i < program->num_philos)
	{
		program->philos[i].id = i + 1;
		program->philos[i].meals_count = 0;
		program->philos[i].last_meal_ms = get_time();
		program->philos[i].prog = program;
		program->philos[i].l_fork = &program->forks[i];
		program->philos[i].r_fork = &program->forks[(i + 1)
			% program->num_philos];
		i++;
	}
	return (true);
}

bool	init(t_sim *prog, int argc, char **argv)
{
	prog->has_death = false;
	prog->start_ms = get_time();
	if (!parse_arguments(prog, argc, argv))
		return (false);
	if (!init_mutexes(prog))
		return (false);
	if (!init_philosophers(prog))
		return (false);
	return (true);
}
