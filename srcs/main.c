/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 19:33:26 by sudas             #+#    #+#             */
/*   Updated: 2025/10/07 23:41:47 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void* philo_routine(void* arg)
{
    t_thread	*philo;

	philo = (t_thread *)arg;
	print_philo_state(*philo);
    return NULL;
}

int	print_philo_state(t_thread philo)
{
	if (philo.eating.ans)
	{
		printf("%ld.%ld %d is eating\n", philo.eating.t_sec
			, philo.eating.t_usec, philo.num);
	}
	if (philo.sleeping.ans)
	{
		printf("%ld.%ld %d is sleeping\n", philo.sleeping.t_sec
			, philo.sleeping.t_usec, philo.num);
	}
	if (philo.thinking.ans)
	{
		printf("%ld.%ld %d is thinking\n", philo.thinking.t_sec
			, philo.thinking.t_usec, philo.num);
	}
	if (philo.born.ans)
	{
		printf("%ld.%ld %d is born\n", philo.born.t_sec
			, philo.born.t_usec, philo.num);
	}
	if (philo.dead.ans)
	{
		printf("%ld.%ld %d is dead\n", philo.dead.t_sec
			, philo.dead.t_usec, philo.num);
	}
	return (1);
}


int	start_eating(t_thread philo, t_mutex *fork
		,int philos, long int eat_time)
{
	t_eval tv;

	philo.sleeping.ans = 0;
	philo.dead.ans = 0;
	philo.thinking.ans = 0;
	philo.eating.ans = 1;
	pthread_mutex_lock(&fork[philo.num].init);
	fork[philo.num].inuse = 1;
	pthread_mutex_lock(&fork[(philo.num + 1) % philos].init);
	fork[(philo.num + 1) % philos].inuse = 1;
	print_philo_state(philo);
	usleep(eat_time);
	gettimeofday(&tv, NULL);
	philo.eating.t_sec = tv.tv_sec;
	philo.eating.t_usec = tv.tv_usec;
	philo.sleeping.t_sec = tv.tv_sec;
	philo.sleeping.t_usec = tv.tv_usec;
	philo.sleeping.ans = 1;
	philo.eating.ans = 0;
	pthread_mutex_unlock(&fork[philo.num].init);
	fork[philo.num].inuse = 0;
	pthread_mutex_unlock(&fork[(philo.num + 1) % philos].init);
	fork[(philo.num + 1) % philos].inuse = 0;
	print_philo_state(philo);
	return (0);
}

int	init_philos(t_thread *philo, int num)
{
	int		i;
	t_eval	tv;

	i = 0;
	while (i < num)
	{
		philo[i].num = i;
		philo[i].philos = num;
		philo[i].eating.ans = 0;
		philo[i].born.ans = 1;
		philo[i].sleeping.ans = 0;
		philo[i].thinking.ans = 0;
		philo[i].dead.ans = 0;
		pthread_mutex_init(&philo[i].fork.init, NULL);
		philo[i].fork.inuse = 0;
		gettimeofday(&tv, NULL);
		philo[i].born.t_sec = tv.tv_sec;
		philo[i].born.t_usec = tv.tv_usec;
		// print_philo_state(philo[i]);
		pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]);
		i++;
	}
	return (0);
}

int main(void) 
{
    int					num_philo = 20;
	// int					sleep = 200;
	// int					eat = 50;
	// int					die	= 500;
	t_thread 			*philo;
	int					i = 0;

	philo = malloc(sizeof(t_thread) * num_philo);
	init_philos(philo, num_philo);
	i = 0;
	while(i < num_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}

	// while (i < num_philo)
	// {
	// 	print_philo_state(philo[i]);
	// 	i++;
	// }
    return 0;
}

