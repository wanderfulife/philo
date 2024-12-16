/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:50 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:03:51 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	cleanup_program(t_program *prog)
{
	int	i;

	if (prog->forks)
	{
		i = 0;
		while (i < prog->philo_count)
		{
			pthread_mutex_destroy(&prog->forks[i]);
			i++;
		}
		free(prog->forks);
	}
	pthread_mutex_destroy(&prog->death_mutex);
	pthread_mutex_destroy(&prog->print_mutex);
	if (prog->philos)
		free(prog->philos);
}

int	main(int argc, char **argv)
{
	t_program	prog;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	prog.forks = NULL;
	prog.philos = NULL;
	prog.philo_count = 0;
	if (!init_program(&prog, argc, argv))
	{
		printf("Error: Initialization failed\n");
		cleanup_program(&prog);
		return (1);
	}
	if (!start_simulation(&prog))
	{
		cleanup_program(&prog);
		printf("Error: Simulation failed\n");
		return (1);
	}
	cleanup_program(&prog);
	return (0);
}
