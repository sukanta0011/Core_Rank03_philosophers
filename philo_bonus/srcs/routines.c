/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:25:02 by sudas             #+#    #+#             */
/*   Updated: 2025/10/14 14:32:45 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*philo_routine(void *arg)
{
	t_process	*philo;
	sem_t		*forks;
	sem_t		*print;

	philo = (t_process *)arg;
	forks = sem_open("/forks", 0);
	print = sem_open("/print", 0);
	while (!philo->dead && !philo->finised)
	{
		p_eat(philo, forks, print);
		p_think(philo, print);
		p_sleep(philo, print);
	}
	return (NULL);
}

// void	*monitor_routine(void *arg)
// {
// 	t_process	*philo;
// 	int			i;
// 	int			philo_finished;

// 	philo = (t_process *)arg;
// 	philo_finished = 0;
// 	while (1)
// 	{
// 		if (!is_alive(&philo[i]))
// 		{
// 			change_state(&philo[i], &philo[i].dead, 1);
// 			print_philo_state(&philo[i], "is dead");
// 			stop_routine(philo);
// 			return (NULL);
// 		}
// 		if (philo->info.fixed_eating && philo->eating.counter >= philo->info.times_to_eat)
// 			change_state(&philo[i], &philo[i].finised, 1);
// 		msleep(5);
// 	}
// }

// void	stop_routine(t_process *philo)
// {
// 	int	i;
// 	int	num;

// 	i = 0;
// 	num = philo[0].info.philos;
// 	while (i < num)
// 	{
// 		kill(philo[i].pid, SIGTERM);
// 		philo[i].dead = 1;
// 		i++;
// 	}
// }

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
