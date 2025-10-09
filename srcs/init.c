/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:14:47 by sudas             #+#    #+#             */
/*   Updated: 2025/10/09 23:25:38 by sudas            ###   ########.fr       */
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

void	init_states(t_thread *philo, t_info info)
{
	int	i;
	
	i = 0;
	while (i < info.philos)
	{
		philo[i].num = i + 1;
		philo[i].info.philos = info.philos;
		philo[i].info.sleeping_time = info.sleeping_time;
		philo[i].info.eating_time = info.eating_time;
		philo[i].info.thinking_time = info.thinking_time;
		philo[i].info.death_time = info.death_time;
		philo[i].info.fixed_eating = info.fixed_eating;
		philo[i].info.times_to_eat = info.times_to_eat;
		philo[i].eating.ans = 0;
		philo[i].sleeping = 0;
		philo[i].thinking = 0;
		philo[i].dead = 0;
		philo[i].finised = 0;
		philo[i].fl = 0;
		philo[i].fr = 0;
		philo[i].eating.counter = 0;
		i++;
	}
}

void	init_start_time(t_thread *philo, t_info info)
{
	int			i;
	t_eval		tv;

	i = 0;
	while (i < info.philos)
	{
		gettimeofday(&tv, NULL);
		philo[i].start.t_sec = tv.tv_sec;
		philo[i].start.t_usec = tv.tv_usec;
		i++;
	}
}

void	init_left_right_fork(t_thread *philo, t_info info)
{
	int			i;
	t_mutex		*fork;
	t_bool		*fork_state;

	fork = malloc(sizeof(t_mutex) * info.philos);
	init_forks(fork, info.philos);
	fork_state = malloc(sizeof(t_bool *) * info.philos);
	i = 0;
	while (i < info.philos)
	{
		fork_state[i] = 0;
		i++;
	}
	i = 0;
	while (i < info.philos)
	{
		philo[i].fork_left = &fork[i];
		philo[i].fork_right = &fork[(i + 1) % info.philos];
		philo[i].fl = &fork_state[i];
		philo[i].fr = &fork_state[(i + 1) % info.philos];
		i++;
	}
}

void	init_lock(t_thread *philo, t_info info)
{
	int			i;
	t_mutex		*print_lock;
	t_mutex		*state_lock;

	print_lock = malloc(sizeof(t_mutex) * 1);
	state_lock = malloc(sizeof(t_mutex) * 1);
	pthread_mutex_init(&print_lock[0], NULL);
	pthread_mutex_init(&state_lock[0], NULL);
	i = 0;
	while (i < info.philos)
	{
		philo[i].print_lock = &print_lock[0];
		philo[i].state_lock = &state_lock[0];
		i++;
	}
}

int	init_philos(t_thread *philo, t_info info)
{
	init_states(philo, info);
	init_start_time(philo, info);
	init_left_right_fork(philo, info);
	init_lock(philo, info);
	return (1);
}

