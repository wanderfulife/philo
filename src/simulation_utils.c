/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:59 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:04:00 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	handle_single_philosopher(t_philo *philo)
{
	print_status(philo, "has taken a fork");
	while (!is_simulation_over(philo->prog))
		usleep(100);
}

static void	run_philosopher_lifecycle(t_philo *philo)
{
	if (philo->id % 2 == 0)
		smart_sleep(philo->prog->time_to_eat / 2);
	while (!is_simulation_over(philo->prog))
	{
		check_and_mark_death(philo->prog, philo->id - 1);
		if (is_simulation_over(philo->prog))
			break ;
		philosopher_eat(philo);
		philosopher_sleep(philo);
		philosopher_think(philo);
		if (philo->prog->philo_count % 2 == 1)
			smart_sleep(50);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->prog->print_mutex);
	pthread_mutex_unlock(&philo->prog->print_mutex);
	if (philo->prog->philo_count == 1)
	{
		handle_single_philosopher(philo);
		return (NULL);
	}
	run_philosopher_lifecycle(philo);
	return (NULL);
}
