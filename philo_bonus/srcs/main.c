/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 19:33:26 by sudas             #+#    #+#             */
/*   Updated: 2025/10/15 10:42:49 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_info(int argc, char **argv, t_info *info)
{
	int	extra_time;

	if (argc < 5)
		return (0);
	if (!str_to_unum(argv[1], &info->philos))
		return (0);
	if (!str_to_unum(argv[2], &info->death_time))
		return (0);
	if (!str_to_unum(argv[3], &info->eating_time))
		return (0);
	if (!str_to_unum(argv[4], &info->sleeping_time))
		return (0);
	info->thinking_time = 0;
	info->fixed_eating = 0;
	if (argc == 6)
	{
		info->fixed_eating = 1;
		if (!str_to_unum(argv[5], &info->times_to_eat))
			return (0);
	}
	extra_time = info->death_time - (info->eating_time + info->sleeping_time);
	if (extra_time > 10)
		info->thinking_time = extra_time / 2;
	return (1);
}

int	init_philos(t_process *philo, t_info *info)
{
	init_states(philo, info);
	init_start_time(philo, info);
	init_lock(philo, info);
	return (1);
}

void	free_memory(t_process *philo, t_info *info)
{
	int	i;

	i = 0;
	while (i < info[0].philos)
	{
		pthread_mutex_destroy(philo[i].state_lock);
		i++;
	}
	free(philo[0].state_lock);
	free(philo);
}

void	free_sem()
{
	sem_unlink("/print");
	sem_unlink("/forks");
	sem_unlink("/dead");
	sem_unlink("/finished");
}

int	main(int argc, char **argv)
{
	t_info		*info;
	t_process	*philo;
	int			i;
	pid_t		pid;
	t_lock		lock;
	int			finished;

	info = malloc(sizeof(t_info));
	if (!info)
		return (0);
	if (init_info(argc, argv, &info[0]))
	{
		philo = malloc(sizeof(t_process) * info[0].philos);
		init_philos(philo, &info[0]);
		init_sems(info->philos);
		finished = 0;
		lock.dead = sem_open("/dead", 0);
		lock.finished = sem_open("/finished", 0);
		i = -1;
		while (++i < info[0].philos)
		{
			pid = fork();
			if (pid == 0)
			{
				philo_routine(&philo[i]);
				exit(0);
			}
			else
				philo[i].pid = pid;
		}
		if (info->fixed_eating)
		{
			i = -1;
			while (++i < info[0].philos)
			{
				sem_wait(lock.finished);
				finished++;
			}
		}
		else
		{
			sem_wait(lock.dead);
			kill_all_processes(philo);
		}
		i = -1;
		while (++i < info[0].philos)
			waitpid(philo[i].pid, NULL, 0);
		free_sem();
		free_memory(philo, info);
	}
	free(info);
	return (0);
}
