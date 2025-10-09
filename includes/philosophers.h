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
}				t_state;

typedef struct	s_info
{
	int		philos;
	int 	eating_time;
	int		sleeping_time;
	int		thinking_time;
	int		death_time;
	t_bool	fixed_eating;
	int		times_to_eat;
}				t_info;

typedef struct	s_lock
{
	t_mutex		print;
	t_mutex		state;
}				t_lock;

typedef struct s_thread
{
	pthread_t	thread;
	int			num;
	t_info		info;
	t_state		start;
	t_state		eating;
	t_bool 		sleeping;
	t_bool 		thinking;
	t_bool		finised;
	t_bool		dead;
	t_bool		fl;
	t_bool		fr;
	t_mutex		*fork_left;
	t_mutex		*fork_right;
	t_mutex		*print_lock;
	t_mutex		*state_lock;
}				t_thread;

void	print_philo_state(t_thread *philo, char *msg);
void	filter_philo_state_changed(t_thread *philo, t_bool *state);

int		init_forks(t_mutex *fork, int num);
int		init_philos(t_thread *philo, t_info info);
// int		init_philos(t_thread *philo, t_mutex *fork, t_mutex	*print_lock
// 			, t_mutex *state_lock, t_info info);

void	*philo_routine(void* arg);
void	*monitor_routine(void* arg);
int		is_alive(t_thread *philo);
void	change_state(t_thread *philo, t_bool *state, t_bool ans);
void	set_time(t_thread *philo, t_state *state, t_bool ans);
void	stop_routine(t_thread *philo);
void	p_eat(t_thread *philo);
void	p_think(t_thread *philo);
void	p_sleep(t_thread *philo);

#endif