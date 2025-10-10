/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:26:03 by sudas             #+#    #+#             */
/*   Updated: 2025/10/10 11:55:19 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	msleep(long int msec)
{
	t_eval		tv;
	long int	start_sec;
	long int	start_usec;
	long int	dmsec;

	gettimeofday(&tv, NULL);
	start_sec = tv.tv_sec;
	start_usec = tv.tv_usec;
	dmsec = 0;
	while (dmsec < msec)
	{
		usleep(100);
		gettimeofday(&tv, NULL);
		dmsec = (tv.tv_sec - start_sec) * 1000
			+ (tv.tv_usec - start_usec) / 1000;
	}
	return ;
}

void	change_state(t_thread *philo, t_bool *state, t_bool ans)
{
	pthread_mutex_lock(philo->state_lock);
	*state = ans;
	filter_philo_state_changed(philo, state);
	pthread_mutex_unlock(philo->state_lock);
}

void	set_time(t_thread *philo, t_state *state, t_bool ans)
{
	t_eval	tv;

	pthread_mutex_lock(philo->state_lock);
	state->ans = ans;
	gettimeofday(&tv, NULL);
	state->t_sec = tv.tv_sec;
	state->t_usec = tv.tv_usec;
	philo->eating.counter++;
	print_philo_state(philo, "is eating");
	printf("Philosopher %d is eating for %d times\n", philo->num,
		philo->eating.counter);
	pthread_mutex_unlock(philo->state_lock);
}

void	p_eat(t_thread *philo)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		if (philo->num % 2 == 0)
			pthread_mutex_lock(philo->fork_left);
		else
			pthread_mutex_lock(philo->fork_right);
		print_philo_state(philo, "has taken first fork");
		if (philo->num % 2 == 0)
			pthread_mutex_lock(philo->fork_right);
		else
			pthread_mutex_lock(philo->fork_left);
		print_philo_state(philo, "has taken second fork");
		set_time(philo, &philo->eating, 1);
		msleep(philo->info.eating_time);
		change_state(philo, &philo->eating.ans, 0);
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_unlock(philo->fork_left);
	}
}

void	p_think(t_thread *philo)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		change_state(philo, &philo->thinking, 1);
		msleep(philo->info.thinking_time);
		change_state(philo, &philo->thinking, 0);
	}
}

void	p_sleep(t_thread *philo)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		change_state(philo, &philo->sleeping, 1);
		msleep(philo->info.sleeping_time);
		change_state(philo, &philo->sleeping, 0);
	}
}
