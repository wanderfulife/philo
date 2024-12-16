/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:50 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:49:54 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	cleanup_program(t_sim *prog)
{
	int	i;

	if (prog->forks)
	{
		i = 0;
		while (i < prog->num_philos)
		{
			pthread_mutex_destroy(&prog->forks[i]);
			i++;
		}
		free(prog->forks);
	}
	pthread_mutex_destroy(&prog->death_lock);
	pthread_mutex_destroy(&prog->print_lock);
	if (prog->philos)
		free(prog->philos);
}

int	main(int argc, char **argv)
{
	t_sim	prog;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	prog.forks = NULL;
	prog.philos = NULL;
	prog.num_philos = 0;
	if (!init(&prog, argc, argv))
	{
		printf("Error: Initialization failed\n");
		cleanup_program(&prog);
		return (1);
	}
	if (!start(&prog))
	{
		cleanup_program(&prog);
		printf("Error: Simulation failed\n");
		return (1);
	}
	cleanup_program(&prog);
	return (0);
}
