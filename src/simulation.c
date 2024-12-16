/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:04:03 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:52:31 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	check_all_ate_enough(t_sim *prog)
{
	int		i;
	bool	all_done;

	if (prog->meals_limit == -1)
		return (false);
	pthread_mutex_lock(&prog->death_lock);
	i = 0;
	all_done = true;
	while (i < prog->num_philos)
	{
		if (prog->philos[i].meals_count < prog->meals_limit)
		{
			all_done = false;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&prog->death_lock);
	return (all_done);
}

bool	it_sim_done(t_sim *prog)
{
	bool	should_stop;
	bool	has_death;

	pthread_mutex_lock(&prog->death_lock);
	has_death = prog->has_death;
	pthread_mutex_unlock(&prog->death_lock);
	should_stop = has_death;
	if (!should_stop)
		should_stop = check_all_ate_enough(prog);
	return (should_stop);
}

static bool	init_threads(t_sim *prog, pthread_t *threads)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&prog->print_lock);
	while (i < prog->num_philos)
	{
		if (pthread_create(&threads[i], NULL, philo_routine,
				&prog->philos[i]) != 0)
			return (false);
		i++;
		msleep(1);
	}
	pthread_mutex_unlock(&prog->print_lock);
	return (true);
}

static bool	join_threads(t_sim *prog, pthread_t *threads, pthread_t monitor)
{
	int	i;

	if (pthread_create(&monitor, NULL, monitor_death, prog) != 0)
		return (false);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < prog->num_philos)
		pthread_join(threads[i++], NULL);
	return (true);
}

bool	start(t_sim *prog)
{
	pthread_t	*threads;
	pthread_t	monitor;
	bool		success;

	threads = malloc(sizeof(pthread_t) * prog->num_philos);
	if (!threads)
		return (false);
	prog->start_ms = get_time();
	monitor = 0;
	success = init_threads(prog, threads) && join_threads(prog, threads,
			monitor);
	free(threads);
	return (success);
}
