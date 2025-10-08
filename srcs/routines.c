/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:25:02 by sudas             #+#    #+#             */
/*   Updated: 2025/10/08 16:44:10 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void* arg)
{
    t_thread	*philo;

	philo = (t_thread *)arg;
	while (!philo->dead.ans)
	{
		p_eat(philo);
		p_think(philo);
		p_sleep(philo);
	}
    return NULL;
}

void	*monitor_routine(void* arg)
{
    t_thread	*philo;
	int			i;
	int			philo_num;
	int			run;

	run = 1;
	philo = (t_thread *)arg;
	philo_num = philo[0].info.philos;
	while (run)
	{
		i = 0;
		while (i < philo_num)
		{
			pthread_mutex_lock(philo[i].print_lock);
			if (!is_alive(&philo[i]))
			{
				change_state(&philo[i], &philo[i].dead, 1);
				// pthread_mutex_unlock(philo[i].print_lock);
				// print_philo_state(philo[i], "is dead");
				run = 0;
				return (NULL);
			}
			pthread_mutex_unlock(philo[i].print_lock);
			i++;
		}
		usleep(1000);
	}
	stop_routine(philo);
    return (NULL);
}

void	stop_routine(t_thread *philo)
{
	int i;
	int	num;

	num = philo[0].info.philos;
	i = 0;
	while(i < num)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

int	is_alive(t_thread *philo)
{
	t_eval 		tv;
	long int	dt_msec;

	gettimeofday(&tv, NULL);
	if (philo->eating.counter == 0)
	{
		dt_msec = (tv.tv_sec - philo->start.t_sec) * 1000
			+ (tv.tv_usec - philo->start.t_usec) / 1000;
	}
	else
	{
		dt_msec = (tv.tv_sec - philo->eating.t_sec) * 1000
			+ (tv.tv_usec - philo->eating.t_usec) / 1000;
	}
	if (dt_msec > philo->info.death_time)
	{
		printf("philo %d dead, %ld", philo->num, dt_msec);
		return (0);
	}
	return (1);
}

void	change_state(t_thread *philo, t_state *state, t_bool ans)
{
	t_eval tv;
	
	state->ans = ans;
	gettimeofday(&tv, NULL);
	state->t_sec = tv.tv_sec;
	state->t_usec = tv.tv_usec;
	filter_philo_state(*philo);
}

void	p_eat(t_thread *philo)
{
	// if (!is_alive(philo))
	// {
	// 	change_state(philo, &philo->dead, 1);
	// 	print_philo_state(*philo, "is dead");
	// 	stop_routine(philo);
	// }
	if (!philo->fl.ans && !philo->fr.ans && !philo->sleeping.ans
			&& !philo->thinking.ans && !philo->eating.ans && !philo->dead.ans)
	{

		pthread_mutex_lock(philo->fork_left);
		change_state(philo, &philo->fl, 1);
		pthread_mutex_lock(philo->fork_right);
		change_state(philo, &philo->fr, 1);
		change_state(philo, &philo->eating, 1);
		philo->eating.counter++;
		usleep(philo->info.eating_time * 1000);
		philo->eating.ans = 0;
		philo->fl.ans = 0;
		philo->fr.ans = 0;
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
	}
}

void	p_think(t_thread *philo)
{
	
	if (!philo->fl.ans && !philo->fr.ans && !philo->sleeping.ans
			&& !philo->thinking.ans && !philo->eating.ans && !philo->dead.ans)
	{
		change_state(philo, &philo->thinking, 1);
		usleep(philo->info.thinking_time * 1000);
		philo->thinking.ans = 0;
	}
}

void	p_sleep(t_thread *philo)
{
	if (!philo->fl.ans && !philo->fr.ans && !philo->sleeping.ans
			&& !philo->thinking.ans && !philo->eating.ans && !philo->dead.ans)
	{
		change_state(philo, &philo->sleeping, 1);
		usleep(philo->info.sleeping_time * 1000);
		philo->sleeping.ans = 0;
	}
}
