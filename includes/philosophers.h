/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:29 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:52:31 by JoWander         ###   ########.fr       */
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

# define COLOR_RESET "\x1B[0m"
# define COLOR_EAT "\x1B[32m"   // Green for eating
# define COLOR_SLEEP "\x1B[36m" // Cyan for sleeping
# define COLOR_DIE "\x1B[31m"   // Red for dying
# define COLOR_FORK "\x1B[33m"  // Yellow for fork actions
# define COLOR_THINK "\x1B[35m" // Magenta for thinking

typedef struct s_program	t_sim;
typedef struct s_philo		t_philo;

typedef struct s_philo
{
	int						id;
	pthread_mutex_t			*l_fork;
	pthread_mutex_t			*r_fork;
	long long				last_meal_ms;
	int						meals_count;
	t_sim					*prog;
}							t_philo;

typedef struct s_program
{
	t_philo					*philos;
	pthread_mutex_t			*forks;
	pthread_mutex_t			death_lock;
	pthread_mutex_t			print_lock;
	bool					has_death;
	int						num_philos;
	long long				start_ms;
	int						die_ms;
	int						eat_ms;
	int						sleep_ms;
	int						meals_limit;
}							t_sim;

// Core functions
bool						init(t_sim *prog, int argc, char **argv);
bool						start(t_sim *prog);
void						*philo_routine(void *arg);

// Utils
long long					get_time(void);
void						msleep(long long ms);
void						log_status(t_philo *philo, char *status);
bool						is_valid_num(char *str);

bool						it_sim_done(t_sim *prog);

// Philosopher routines
bool						philo_eat(t_philo *philo);
void						philo_sleep(t_philo *philo);
void						philo_think(t_philo *philo);
void						check_death(t_sim *prog, int i);
void						*monitor_death(void *arg);

// Color utils
const char					*get_status_color(char *status);
void						print_colored_status(t_philo *philo, char *status,
								long long timestamp);

#endif
