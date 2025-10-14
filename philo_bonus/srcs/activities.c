/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:26:03 by sudas             #+#    #+#             */
/*   Updated: 2025/10/14 15:15:07 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	change_state(t_process *philo, t_bool *state, t_bool ans)
{
	pthread_mutex_lock(philo->state_lock);
	*state = ans;
	filter_philo_state_changed(philo, state);
	pthread_mutex_unlock(philo->state_lock);
}

void	set_time(t_process *philo, t_state *state, t_bool ans)
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

void	p_eat(t_process *philo)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		sem_wait(philo->lock.forks);
		sem_wait(philo->lock.forks);
		set_time(philo, &philo->eating, 1);
		msleep(philo->info.eating_time);
		change_state(philo, &philo->eating.ans, 0);
		sem_post(philo->lock.forks);
		sem_post(philo->lock.forks);
	}
}

void	p_think(t_process *philo)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		change_state(philo, &philo->thinking, 1);
		msleep(philo->info.thinking_time);
		change_state(philo, &philo->thinking, 0);
	}
}

void	p_sleep(t_process *philo)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		change_state(philo, &philo->sleeping, 1);
		msleep(philo->info.sleeping_time);
		change_state(philo, &philo->sleeping, 0);
	}
}
