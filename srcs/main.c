/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 19:33:26 by sudas             #+#    #+#             */
/*   Updated: 2025/10/09 15:53:43 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int main(void) 
{
	t_info		info;
	t_thread 	*philo;
	pthread_t	monitor;
	int			i;

	info.philos = 3;
	info.death_time = 700;
	info.sleeping_time = 200;
	info.eating_time = 200;
	info.thinking_time = 200;
	info.fixed_eating = 0;
	info.times_to_eat = 5;
	philo = malloc(sizeof(t_thread) * info.philos);
	init_philos(philo, info);
	i = 0;
	while (i < info.philos)
	{
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		usleep(1000);
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
	pthread_mutex_destroy(philo[0].fork_left);
	free(philo[0].fork_left);
	i++;
	pthread_mutex_destroy(philo[0].print_lock);
	pthread_mutex_destroy(philo[0].state_lock);
	free(philo[0].print_lock);
	free(philo[0].state_lock);
	free(philo);
    return (0);
}
