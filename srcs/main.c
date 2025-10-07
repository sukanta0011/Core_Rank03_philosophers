#include "philosophers.h"

void* foo(void* arg) 
{
    int	*t_num;

	t_num = (int *)arg;
	printf("Thread %d is running.\n", *t_num);
    return NULL;
}

int	print_philo_state(t_thread philo)
{
	if (philo.eating.ans)
	{
		printf("%ld.%ld %d is earing\n", philo.eating.t_sec
			, philo.eating.t_usec, philo.num);
	}
	if (philo.sleeping.ans)
	{
		printf("%ld.%ld %d is sleeping\n", philo.sleeping.t_sec
			, philo.sleeping.t_usec, philo.num);
	}
	if (philo.ideal.ans)
	{
		printf("%ld.%ld %d is ideal\n", philo.ideal.t_sec
			, philo.ideal.t_usec, philo.num);
	}
	if (philo.died.ans)
	{
		printf("%ld.%ld %d is ideal\n", philo.died.t_sec
			, philo.died.t_usec, philo.num);
	}
	return (1);
}

int	init_forks(pthread_mutex_t *fork, int num)
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

int	init_philos(t_thread *philo, int num)
{
	int		i;
	t_eval tv;

	i = 0;
	while (i < num)
	{
		philo[i].num = i;
		philo[i].eating.ans = 0;
		philo[i].sleeping.ans = 0;
		philo[i].died.ans = 0;
		philo[i].ideal.ans = 1;
		gettimeofday(&tv, NULL);
		philo[i].ideal.t_sec = tv.tv_sec;
		philo[i].ideal.t_usec = tv.tv_usec;
		i++;
	}
	return (0);
}

int main(void) 
{
    int					num_philo = 5;
	int					sleep = 200;
	int					eat = 50;
	// int					die	= 500;
	pthread_mutex_t		*fork;
	t_thread 			*philo;
	int					i = 0;

	fork = malloc(sizeof(pthread_mutex_t) * num_philo);
	philo = malloc(sizeof(t_thread) * num_philo);
	init_forks(fork, num_philo);
	init_philos(philo, num_philo);
	pthread_create(&philo[0].thread, NULL, foo, &sleep);
    pthread_create(&philo[1].thread, NULL, foo, &eat);
	pthread_join(philo[0].thread, NULL);
    pthread_join(philo[1].thread, NULL);

	while (i < num_philo)
	{
		print_philo_state(philo[i]);
		i++;
	}

    return 0;
}

