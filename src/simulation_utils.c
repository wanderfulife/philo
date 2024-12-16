/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:59 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:50:46 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	handle_single_philosopher(t_philo *philo)
{
	log_status(philo, "has taken a fork");
	while (!it_sim_done(philo->prog))
		usleep(100);
}

static void	run_philosopher_lifecycle(t_philo *philo)
{
	if (philo->id % 2 == 0)
		msleep(philo->prog->eat_ms / 2);
	while (!it_sim_done(philo->prog))
	{
		check_death(philo->prog, philo->id - 1);
		if (it_sim_done(philo->prog))
			break ;
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
		if (philo->prog->num_philos % 2 == 1)
			msleep(50);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->prog->print_lock);
	pthread_mutex_unlock(&philo->prog->print_lock);
	if (philo->prog->num_philos == 1)
	{
		handle_single_philosopher(philo);
		return (NULL);
	}
	run_philosopher_lifecycle(philo);
	return (NULL);
}
