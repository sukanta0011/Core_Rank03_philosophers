#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct timeval t_eval;
typedef struct timezone	t_zone;

typedef struct	s_state
{
	int			ans;
	long int	t_sec;
	long int	t_usec;
}			t_state;

typedef struct s_mutex
{
	pthread_mutex_t init;
	int				inuse;
}				t_mutex;

typedef struct s_thread
{
	pthread_t	thread;
	int			num;
	int			philos;
	t_state		born;
	t_state		eating;
	t_state 	sleeping;
	t_state 	thinking;
	t_state		dead;
	t_mutex		fork;
}				t_thread;


void* foo(void* arg);
int	print_philo_state(t_thread philo);
int	init_forks(t_mutex *fork, int num);
int	init_philos(t_thread *philo, int num);

#endif