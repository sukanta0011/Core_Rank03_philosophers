/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:14:47 by sudas             #+#    #+#             */
/*   Updated: 2025/10/14 13:30:01 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	init_states(t_process *philo, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->philos)
	{
		philo[i].num = i + 1;
		philo[i].info.philos = info->philos;
		philo[i].info.sleeping_time = info->sleeping_time;
		philo[i].info.eating_time = info->eating_time;
		philo[i].info.thinking_time = info->thinking_time;
		philo[i].info.death_time = info->death_time;
		philo[i].info.fixed_eating = info->fixed_eating;
		philo[i].info.times_to_eat = info->times_to_eat;
		philo[i].eating.ans = 0;
		philo[i].sleeping = 0;
		philo[i].thinking = 0;
		philo[i].dead = 0;
		philo[i].finised = 0;
		philo[i].eating.counter = 0;
		i++;
	}
}

void	init_start_time(t_process *philo, t_info *info)
{
	int			i;
	t_eval		tv;

	i = 0;
	while (i < info->philos)
	{
		gettimeofday(&tv, NULL);
		philo[i].start.t_sec = tv.tv_sec;
		philo[i].start.t_usec = tv.tv_usec;
		i++;
	}
}

void	init_lock(t_process *philo, t_info *info)
{
	int			i;
	t_mutex		*state_lock;

	state_lock = malloc(sizeof(t_mutex) * info->philos);
	i = 0;
	while (i < info->philos)
	{
		pthread_mutex_init(&state_lock[i], NULL);
		philo[i].state_lock = &state_lock[i];
		i++;
	}
}

void	init_sems(int num)
{
	sem_unlink("/print");
	sem_unlink("/forks");
	sem_open("/print", O_CREAT, 0644, 1);
	sem_open("/forks", O_CREAT, 0644, num);
}
