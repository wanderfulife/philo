/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:04:03 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:04:04 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	check_all_ate_enough(t_program *prog)
{
	int		i;
	bool	all_done;

	if (prog->must_eat_count == -1)
		return (false);
	pthread_mutex_lock(&prog->death_mutex);
	i = 0;
	all_done = true;
	while (i < prog->philo_count)
	{
		if (prog->philos[i].meals_eaten < prog->must_eat_count)
		{
			all_done = false;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&prog->death_mutex);
	return (all_done);
}

bool	is_simulation_over(t_program *prog)
{
	bool	should_stop;
	bool	someone_died;

	pthread_mutex_lock(&prog->death_mutex);
	someone_died = prog->someone_died;
	pthread_mutex_unlock(&prog->death_mutex);
	should_stop = someone_died;
	if (!should_stop)
		should_stop = check_all_ate_enough(prog);
	return (should_stop);
}

static bool	init_threads(t_program *prog, pthread_t *threads)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&prog->print_mutex);
	while (i < prog->philo_count)
	{
		if (pthread_create(&threads[i], NULL, philosopher_routine,
				&prog->philos[i]) != 0)
			return (false);
		i++;
		smart_sleep(1);
	}
	pthread_mutex_unlock(&prog->print_mutex);
	return (true);
}

static bool	join_threads(t_program *prog, pthread_t *threads,
	pthread_t monitor)
{
	int	i;

	if (pthread_create(&monitor, NULL, death_monitor, prog) != 0)
		return (false);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < prog->philo_count)
		pthread_join(threads[i++], NULL);
	return (true);
}

bool	start_simulation(t_program *prog)
{
	pthread_t	*threads;
	pthread_t	monitor;
	bool		success;

	threads = malloc(sizeof(pthread_t) * prog->philo_count);
	if (!threads)
		return (false);
	prog->start_time = get_time();
	monitor = 0;
	success = init_threads(prog, threads)
		&& join_threads(prog, threads, monitor);
	free(threads);
	return (success);
}
