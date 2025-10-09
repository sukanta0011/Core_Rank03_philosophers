/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:25:02 by sudas             #+#    #+#             */
/*   Updated: 2025/10/09 15:45:09 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void* arg)
{
    t_thread	*philo;

	philo = (t_thread *)arg;
	while (!philo->dead && !philo->finised)
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
	t_bool		alive;
	int			philo_num;
	int			philo_finished;

	philo = (t_thread *)arg;
	philo_num = philo[0].info.philos;
	philo_finished = 0;
	while (1)
	{
		i = 0;
		while (i < philo_num)
		{
			pthread_mutex_lock(philo->state_lock);
			alive = is_alive(&philo[i]);
			pthread_mutex_unlock(philo->state_lock);
			if (!alive)
			{
				change_state(&philo[i], &philo[i].dead, 1);
        		print_philo_state(&philo[i], "is dead");
				stop_routine(philo);
				return (NULL);
			}
			if (philo[i].info.fixed_eating && !philo[i].finised
				&& (philo[i].eating.counter >= philo[i].info.times_to_eat))
			{
				change_state(&philo[i], &philo[i].finised, 1);
				philo_finished++;
				if (philo_finished >= philo_num)
					return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
    return (NULL);
}

void	stop_routine(t_thread *philo)
{
	int i;
	int	num;

	i = 0;
	num = philo[0].info.philos;
	while(i < num)
	{
		change_state(&philo[i], &philo[i].dead, 1);
		philo[i].dead = 1;
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
		printf("%d Hunger time: %ld, sec: %ld, usec: %ld\n",philo->num, dt_msec, (tv.tv_sec - philo->eating.t_sec), (tv.tv_usec - philo->eating.t_usec));
		return (0);
	}
	return (1);
}
