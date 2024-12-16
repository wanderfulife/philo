/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:54 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:52:31 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

bool	philo_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (it_sim_done(philo->prog))
		return (false);
	if (philo->id % 2 == 0)
	{
		first = philo->r_fork;
		second = philo->l_fork;
	}
	else
	{
		first = philo->l_fork;
		second = philo->r_fork;
	}
	(pthread_mutex_lock(first), log_status(philo, "has taken a fork"));
	(pthread_mutex_lock(second), log_status(philo, "has taken a fork"));
	log_status(philo, "is eating");
	pthread_mutex_lock(&philo->prog->death_lock);
	philo->last_meal_ms = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->prog->death_lock);
	msleep(philo->prog->eat_ms);
	(pthread_mutex_unlock(second), pthread_mutex_unlock(first));
	return (true);
}

void	philo_sleep(t_philo *philo)
{
	if (!it_sim_done(philo->prog))
	{
		log_status(philo, "is sleeping");
		msleep(philo->prog->sleep_ms);
	}
}

void	philo_think(t_philo *philo)
{
	if (!it_sim_done(philo->prog))
		log_status(philo, "is thinking");
}
