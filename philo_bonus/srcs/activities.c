/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:26:03 by sudas             #+#    #+#             */
/*   Updated: 2025/10/14 14:24:22 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	change_state(t_process *philo, t_bool *state, t_bool ans, sem_t *print)
{
	pthread_mutex_lock(philo->state_lock);
	*state = ans;
	filter_philo_state_changed(philo, state, print);
	pthread_mutex_unlock(philo->state_lock);
}

void	set_time(t_process *philo, t_state *state, t_bool ans, sem_t *print)
{
	t_eval	tv;

	pthread_mutex_lock(philo->state_lock);
	state->ans = ans;
	gettimeofday(&tv, NULL);
	state->t_sec = tv.tv_sec;
	state->t_usec = tv.tv_usec;
	philo->eating.counter++;
	print_philo_state(philo, "is eating", print);
	printf("Philosopher %d is eating for %d times\n", philo->num,
		philo->eating.counter);
	pthread_mutex_unlock(philo->state_lock);
}

void	p_eat(t_process *philo, sem_t *forks, sem_t *print)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		sem_wait(forks);
		sem_wait(forks);
		set_time(philo, &philo->eating, 1, print);
		msleep(philo->info.eating_time);
		change_state(philo, &philo->eating.ans, 0, print);
		sem_post(forks);
		sem_post(forks);
	}
}

void	p_think(t_process *philo, sem_t *print)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		change_state(philo, &philo->thinking, 1, print);
		msleep(philo->info.thinking_time);
		change_state(philo, &philo->thinking, 0, print);
	}
}

void	p_sleep(t_process *philo, sem_t *print)
{
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		change_state(philo, &philo->sleeping, 1, print);
		msleep(philo->info.sleeping_time);
		change_state(philo, &philo->sleeping, 0, print);
	}
}
