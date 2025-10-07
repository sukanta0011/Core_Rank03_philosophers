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

typedef struct s_thread
{
	pthread_t	thread;
	int			num;
	t_state		eating;
	t_state 	sleeping;
	t_state 	ideal;
	t_state		died;
}				t_thread;

#endif