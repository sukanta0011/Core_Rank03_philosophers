/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 19:33:26 by sudas             #+#    #+#             */
/*   Updated: 2025/10/08 16:04:52 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_forks(t_mutex *fork, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_init(&fork[i], NULL);
		i++;
	}
	return (1);
}

int	init_philos(t_thread *philo, t_mutex *fork, t_mutex *print_lock, t_info info)
{
	int		i;
	t_eval	tv;

	(void)print_lock;
	i = 0;
	while (i < info.philos)
	{
		philo[i].num = i + 1;
		philo[i].info.philos = info.philos;
		philo[i].info.sleeping_time = info.sleeping_time;
		philo[i].info.eating_time = info.eating_time;
		philo[i].info.thinking_time = info.thinking_time;
		philo[i].info.death_time = info.death_time;
		philo[i].fork_left = &fork[i];
		philo[i].fork_right = &fork[(i + 1) % info.philos];
		philo[i].eating.ans = 0;
		philo[i].sleeping.ans = 0;
		philo[i].thinking.ans = 0;
		philo[i].dead.ans = 0;
		philo[i].fl.ans = 0;
		philo[i].fr.ans = 0;
		gettimeofday(&tv, NULL);
		philo[i].eating.counter = 0;
		philo[i].start.t_sec = tv.tv_sec;
		philo[i].start.t_usec = tv.tv_usec;
		philo[i].print_lock = print_lock;
		i++;
	}
	return (0);
}

int main(void) 
{
	t_info		info;
	t_thread 	*philo;
	t_mutex		*fork;
	t_mutex		print_lock;
	pthread_t	monitor;
	int			i;

	info.philos = 20;
	info.death_time = 100;
	info.sleeping_time = 300;
	info.eating_time = 200;
	info.thinking_time = 400;
	pthread_mutex_init(&print_lock, NULL);
	fork = malloc(sizeof(t_mutex) * info.philos);
	philo = malloc(sizeof(t_thread) * info.philos);
	init_forks(fork, info.philos);
	init_philos(philo, fork, &print_lock, info);
	i = 0;
	while (i < info.philos)
	{
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, philo);
	pthread_join(monitor, NULL);
	i = 0;
	while(i < info.philos)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
    return (0);
}

