#include "philosophers.h"

void	print_philo_state(t_thread philo, char *msg)
{	
	long int	msec;
	t_eval		tv;

	pthread_mutex_lock(philo.print_lock);
	gettimeofday(&tv, NULL);
	msec = (tv.tv_sec - philo.start.t_sec) * 1000
		+ (tv.tv_usec - philo.start.t_usec) / 1000;
	printf("%ld ms, Philosopher %d %s\n", msec, philo.num, msg);
	pthread_mutex_unlock(philo.print_lock);
}

void	filter_philo_state(t_thread philo)
{
	if (philo.fl.ans)
		print_philo_state(philo, "has taken left fork");
	if (philo.fr.ans)
		print_philo_state(philo, "has taken right fork");
	if (philo.eating.ans)
	{
		print_philo_state(philo, "is eating");
		printf("Philosopher %d is eating for %d times\n", philo.num
			, philo.eating.counter);
	}
	if (philo.sleeping.ans)
		print_philo_state(philo, "is sleeping");
	if (philo.thinking.ans)
		print_philo_state(philo, "is thinking");
}