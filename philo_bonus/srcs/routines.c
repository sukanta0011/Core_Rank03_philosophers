/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:25:02 by sudas             #+#    #+#             */
/*   Updated: 2025/10/13 14:51:15 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void *arg)
{
	t_thread	*philo;

	philo = (t_thread *)arg;
	while (!philo->dead && !philo->finised)
	{
		p_eat(philo);
		p_think(philo);
		p_sleep(philo);
	}
	return (NULL);
}

int	all_finished_eating(t_thread *philo, int *i, int *philo_finished)
{
	if (philo[*i].info.fixed_eating && !philo[*i].finised
		&& (philo[*i].eating.counter >= philo[*i].info.times_to_eat))
	{
		change_state(&philo[*i], &philo[*i].finised, 1);
		(*philo_finished)++;
		if ((*philo_finished) >= philo[0].info.philos)
			return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_thread	*philo;
	int			i;
	int			philo_finished;

	philo = (t_thread *)arg;
	philo_finished = 0;
	while (1)
	{
		i = 0;
		while (i < philo[0].info.philos)
		{
			if (!is_alive(&philo[i]))
			{
				change_state(&philo[i], &philo[i].dead, 1);
				print_philo_state(&philo[i], "is dead");
				stop_routine(philo);
				return (NULL);
			}
			if (all_finished_eating(philo, &i, &philo_finished))
				return (NULL);
			i++;
		}
		msleep(5);
	}
}

void	stop_routine(t_thread *philo)
{
	int	i;
	int	num;

	i = 0;
	num = philo[0].info.philos;
	while (i < num)
	{
		change_state(&philo[i], &philo[i].dead, 1);
		philo[i].dead = 1;
		i++;
	}
}

int	is_alive(t_thread *philo)
{
	t_eval		tv;
	long int	dt_msec;

	pthread_mutex_lock(philo->state_lock);
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
		printf ("%d Hunger time: %ld, sec: %ld, usec: %ld\n", philo->num,
			dt_msec, (tv.tv_sec - philo->eating.t_sec),
			(tv.tv_usec - philo->eating.t_usec));
		pthread_mutex_unlock(philo->state_lock);
		return (0);
	}
	pthread_mutex_unlock(philo->state_lock);
	return (1);
}
