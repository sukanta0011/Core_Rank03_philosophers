/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:26:03 by sudas             #+#    #+#             */
/*   Updated: 2025/10/14 16:06:24 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	change_state(t_thread *philo, t_bool *state, t_bool ans)
{
	pthread_mutex_lock(philo->state_lock);
	*state = ans;
	pthread_mutex_unlock(philo->state_lock);
	filter_philo_state_changed(philo, state);
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
	pthread_mutex_unlock(philo->state_lock);
	print_philo_state(philo, "is eating");
	pthread_mutex_lock(philo->print_lock);
	printf("Philosopher %d is eating for %d times\n", philo->num,
		philo->eating.counter);
	pthread_mutex_unlock(philo->print_lock);
}

void	p_eat(t_thread *philo)
{
	t_mutex *first_fork;
	t_mutex *second_fork;
	int left_idx;
	int right_idx;
	
	if (!philo->finised && !philo->sleeping
		&& !philo->thinking && !philo->eating.ans && !philo->dead)
	{
		// Determine fork indices
		left_idx = philo->num - 1;  // Convert to 0-based
		right_idx = (philo->num) % philo->info.philos;
		
		// Always lock lower-numbered fork first
		if (left_idx < right_idx)
		{
			first_fork = philo->fork_left;
			second_fork = philo->fork_right;
		}
		else
		{
			first_fork = philo->fork_right;
			second_fork = philo->fork_left;
		}
		pthread_mutex_lock(philo->print_lock);
		// printf("%ld ms, %d is trying to eat\n", get_current_time(philo), philo->num);
		pthread_mutex_unlock(philo->print_lock);
		
		pthread_mutex_lock(first_fork);
		pthread_mutex_lock(philo->print_lock);
		// printf("%ld ms, %d got first fork\n", get_current_time(philo), philo->num);
		pthread_mutex_unlock(philo->print_lock);
		pthread_mutex_lock(second_fork);
		pthread_mutex_lock(philo->print_lock);
		// printf("%ld ms, %d got second fork\n", get_current_time(philo), philo->num);
		pthread_mutex_unlock(philo->print_lock);
		// Check if died while waiting
		pthread_mutex_lock(philo->state_lock);
		if (philo->dead) {
			pthread_mutex_unlock(philo->state_lock);
			pthread_mutex_unlock(second_fork);
			pthread_mutex_unlock(first_fork);
			return;
		}
		pthread_mutex_unlock(philo->state_lock);
		
		// if (philo->num % 2 == 0)
		// 	pthread_mutex_lock(philo->fork_left);
		// else
		// 	pthread_mutex_lock(philo->fork_right);
		// print_philo_state(philo, "has taken first fork");
		// if (philo->dead) {
		// 	if (philo->num % 2 == 0)
		// 		pthread_mutex_unlock(philo->fork_left);
		// 	else
		// 		pthread_mutex_unlock(philo->fork_right);
		// 	return;
		// }
		// if (philo->num % 2 == 0)
		// 	pthread_mutex_lock(philo->fork_right);
		// else
		// 	pthread_mutex_lock(philo->fork_left);
		// print_philo_state(philo, "has taken second fork");
		// if (philo->dead) {
		// 	pthread_mutex_unlock(philo->fork_right);
		// 	pthread_mutex_unlock(philo->fork_left);
		// 	return;
		// }
		set_time(philo, &philo->eating, 1);
		msleep(philo->info.eating_time);
		change_state(philo, &philo->eating.ans, 0);
		// print_philo_state(philo, "finised_eating");
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
