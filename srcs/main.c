/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 19:33:26 by sudas             #+#    #+#             */
/*   Updated: 2025/10/10 13:35:49 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	free_memory(t_thread *philo, t_info *info)
{	
	int	i;
	
	i = 0;
	while (i < info[0].philos)
	{
		pthread_mutex_destroy(philo[0].fork_left);
		i++;
	}
	free(philo[0].fork_left);
	pthread_mutex_destroy(philo[0].print_lock);
	pthread_mutex_destroy(philo[0].state_lock);
	free(philo[0].print_lock);
	free(philo[0].state_lock);
	free(philo);
}

int	main(int argc, char **argv)
{
	t_info		*info;
	t_thread	*philo;
	pthread_t	monitor;
	int			i;

	info = malloc(sizeof(t_info));
	if (!info)
		return (0);
	if (init_info(argc, argv, &info[0]))
	{
		philo = malloc(sizeof(t_thread) * info[0].philos);
		init_philos(philo, &info[0]);
		i = 0;
		while (i < info[0].philos)
		{
			pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
			i++;
		}
		pthread_create(&monitor, NULL, monitor_routine, philo);
		pthread_join(monitor, NULL);
		i = 0;
		while (i < info[0].philos)
		{
			pthread_join(philo[i].thread, NULL);
			i++;
		}
		i = 0;
		while (i < info[0].philos)
		{
			pthread_mutex_destroy(philo[0].fork_left);
			i++;
		}
		free(philo[0].fork_left);
		pthread_mutex_destroy(philo[0].print_lock);
		pthread_mutex_destroy(philo[0].state_lock);
		free(philo[0].print_lock);
		free(philo[0].state_lock);
		free(philo);
	}
	free(info);
	return (0);
}
