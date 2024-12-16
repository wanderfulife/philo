/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:29 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:03:30 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define COLOR_RESET   "\x1B[0m"
# define COLOR_EAT     "\x1B[32m"  // Green for eating
# define COLOR_SLEEP   "\x1B[36m"  // Cyan for sleeping
# define COLOR_DIE     "\x1B[31m"  // Red for dying
# define COLOR_FORK    "\x1B[33m"  // Yellow for fork actions
# define COLOR_THINK   "\x1B[35m"  // Magenta for thinking

typedef struct s_program	t_program;
typedef struct s_philo		t_philo;

typedef struct s_philo
{
	int						id;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	long long				last_meal_time;
	int						meals_eaten;
	t_program				*prog;
}							t_philo;

typedef struct s_program
{
	t_philo					*philos;
	pthread_mutex_t			*forks;
	pthread_mutex_t			death_mutex;
	pthread_mutex_t			print_mutex;
	bool					someone_died;
	int						philo_count;
	long long				start_time;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						must_eat_count;
}							t_program;

// Core functions
bool						init_program(t_program *prog, int argc,
								char **argv);
bool						start_simulation(t_program *prog);
void						*philosopher_routine(void *arg);

// Utils
long long					get_time(void);
void						smart_sleep(long long ms);
void						print_status(t_philo *philo, char *status);
bool						is_valid_positive_int(char *str);

bool						is_simulation_over(t_program *prog);

// Philosopher routines
bool						philosopher_eat(t_philo *philo);
void						philosopher_sleep(t_philo *philo);
void						philosopher_think(t_philo *philo);
void						check_and_mark_death(t_program *prog, int i);
void						*death_monitor(void *arg);

// Color utils
const char					*get_status_color(char *status);
void						print_colored_status(t_philo *philo,
								char *status, long long timestamp);

#endif
