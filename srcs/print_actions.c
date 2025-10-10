/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:56:07 by sudas             #+#    #+#             */
/*   Updated: 2025/10/10 11:57:04 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_philo_state(t_thread *philo, char *msg)
{
	long int	msec;
	t_eval		tv;

	pthread_mutex_lock(philo->print_lock);
	gettimeofday(&tv, NULL);
	msec = (tv.tv_sec - philo->start.t_sec) * 1000
		+ (tv.tv_usec - philo->start.t_usec) / 1000;
	printf("%ld ms, %d %s\n", msec, philo->num, msg);
	pthread_mutex_unlock(philo->print_lock);
}

void	filter_philo_state_changed(t_thread *philo, t_bool *state)
{
	if (state == &philo->sleeping && *state)
		print_philo_state(philo, "is sleeping");
	else if (state == &philo->thinking && *state)
		print_philo_state(philo, "is thinking");
	else if (state == &philo->finised && *state)
		print_philo_state(philo, "has finished eating");
}
