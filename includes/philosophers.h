#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct timeval t_eval;
typedef struct timezone	t_zone;
typedef int	t_bool;
typedef pthread_mutex_t	t_mutex;

typedef struct	s_state
{
	t_bool		ans;
	int			counter;
	long int	t_sec;
	long int	t_usec;
}			t_state;

typedef struct s_info
{
	int	philos;
	int eating_time;
	int	sleeping_time;
	int	thinking_time;
	int	death_time;
}				t_info;

typedef struct s_thread
{
	pthread_t	thread;
	int			num;
	t_info		info;
	t_state		start;
	t_state		eating;
	t_state 	sleeping;
	t_state 	thinking;
	t_state		dead;
	t_mutex		*fork_left;
	t_mutex		*fork_right;
	t_state		fl;
	t_state		fr;
	t_mutex		*print_lock;
}				t_thread;

void	print_philo_state(t_thread philo, char *msg);
void	filter_philo_state(t_thread philo);

void	*philo_routine(void* arg);
void	*monitor_routine(void* arg);
int		is_alive(t_thread *philo);
void	change_state(t_thread *philo, t_state *state, t_bool ans);
void	stop_routine(t_thread *philo);
void	p_eat(t_thread *philo);
void	p_think(t_thread *philo);
void	p_sleep(t_thread *philo);

int		init_philos(t_thread *philo, t_mutex *fork, t_mutex *print_lock, t_info info);

#endif