/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:41 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:52:19 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	check_death(t_sim *prog, int i)
{
	long long	current_time;
	long long	time_since_meal;

	current_time = get_time();
	pthread_mutex_lock(&prog->death_lock);
	time_since_meal = current_time - prog->philos[i].last_meal_ms;
	if (time_since_meal >= prog->die_ms && !prog->has_death)
	{
		prog->has_death = true;
		pthread_mutex_lock(&prog->print_lock);
		printf("%lld %d died\n", current_time - prog->start_ms,
			prog->philos[i].id);
		pthread_mutex_unlock(&prog->print_lock);
	}
	pthread_mutex_unlock(&prog->death_lock);
}

void	*monitor_death(void *arg)
{
	t_sim	*prog;
	int		i;

	prog = (t_sim *)arg;
	while (!it_sim_done(prog))
	{
		i = 0;
		while (i < prog->num_philos)
		{
			check_death(prog, i);
			pthread_mutex_lock(&prog->death_lock);
			if (prog->has_death)
			{
				pthread_mutex_unlock(&prog->death_lock);
				break ;
			}
			pthread_mutex_unlock(&prog->death_lock);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
