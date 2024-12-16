/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JoWander <jowander@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:03:35 by JoWander          #+#    #+#             */
/*   Updated: 2024/12/16 09:03:37 by JoWander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

const char	*get_status_color(char *status)
{
	if (!strcmp(status, "is eating"))
		return (COLOR_EAT);
	if (!strcmp(status, "is sleeping"))
		return (COLOR_SLEEP);
	if (!strcmp(status, "died"))
		return (COLOR_DIE);
	if (!strcmp(status, "has taken a fork"))
		return (COLOR_FORK);
	if (!strcmp(status, "is thinking"))
		return (COLOR_THINK);
	return (COLOR_RESET);
}

void	print_colored_status(t_philo *philo, char *status,
	long long timestamp)
{
	const char	*color;

	color = get_status_color(status);
	printf("%lld %d %s%s%s\n", timestamp, philo->id,
		color, status, COLOR_RESET);
}
