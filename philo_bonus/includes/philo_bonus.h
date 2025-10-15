/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudas <sudas@student.42prague.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:47:21 by sudas             #+#    #+#             */
/*   Updated: 2025/10/15 10:25:26 by sudas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct timeval	t_eval;
typedef struct timezone	t_zone;
typedef int				t_bool;
typedef pthread_mutex_t	t_mutex;

typedef struct s_state
{
	t_bool		ans;
	int			counter;
	long int	t_sec;
	long int	t_usec;
}				t_state;

typedef struct s_info
{
	long int	philos;
	long int	eating_time;
	long int	sleeping_time;
	long int	thinking_time;
	long int	death_time;
	t_bool		fixed_eating;
	long int	times_to_eat;
}				t_info;

typedef struct s_lock
{
	sem_t		*print;
	sem_t		*forks;
	sem_t		*dead;
	sem_t		*finished;
}				t_lock;

typedef struct s_process
{
	pid_t		pid;
	int			num;
	t_info		info;
	t_state		start;
	t_state		eating;
	t_bool		sleeping;
	t_bool		thinking;
	t_bool		finished;
	t_bool		dead;
	t_mutex		*state_lock;
	t_lock		lock;
}				t_process;

void	print_philo_state(t_process *philo, char *msg);
void	filter_philo_state_changed(t_process *philo, t_bool *state);

void	init_states(t_process *philo, t_info *info);
void	init_start_time(t_process *philo, t_info *info);
void	init_sems(int num);
void	init_lock(t_process *philo, t_info *info);

int		init_philos(t_process *philo, t_info *info);

void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);
void	kill_all_processes(t_process *philo);
int		is_alive(t_process *philo);

void	change_state(t_process *philo, t_bool *state, t_bool ans);
void	set_time(t_process *philo, t_state *state, t_bool ans);
void	p_eat(t_process *philo);
void	p_think(t_process *philo);
void	p_sleep(t_process *philo);

int		str_to_unum(char *str, long int *num);
void	msleep(long int msec);

#endif