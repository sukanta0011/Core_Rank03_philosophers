/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:25:02 by sudas             #+#    #+#             */
/*   Updated: 2025/10/14 16:39:30 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*philo_routine(void *arg)
{
	t_process	*philo;
	pthread_t	monitor;

	philo = (t_process *)arg;
	philo->lock.forks = sem_open("/forks", 0);
	philo->lock.print = sem_open("/print", 0);
	philo->lock.dead = sem_open("/dead", 0);
	philo->lock.finised = sem_open("/finised", 0);
	pthread_create(&monitor, NULL, monitor_routine, philo);
	pthread_detach(monitor);
	while (!philo->dead && !philo->finised)
	{
		p_eat(philo);
		p_think(philo);
		p_sleep(philo);
	}
	exit (1);
}

void	*monitor_routine(void *arg)
{
	t_process	*philo;

	philo = (t_process *)arg;
	while (1)
	{
		if (!is_alive(philo))
		{
			change_state(philo, &philo->dead, 1);
			// sem_wait(philo->lock.print);
			// printf("Philo %d died", philo->num);
			print_philo_state(philo, "is dead");
			sem_post(philo->lock.dead);
			exit(1);
		}
		if (philo->info.fixed_eating && philo->eating.counter >= philo->info.times_to_eat)
		{
			change_state(philo, &philo->finised, 1);
			sem_post(philo->lock.finised);
			return (NULL);
		}
		msleep(5);
	}
}

void	kill_all_processes(t_process *philo)
{
	int	i;
	int	num;

	i = 0;
	num = philo[0].info.philos;
	while (i < num)
	{
		kill(philo[i].pid, SIGTERM);
		philo[i].dead = 1;
		i++;
	}
}

int	is_alive(t_process *philo)
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
