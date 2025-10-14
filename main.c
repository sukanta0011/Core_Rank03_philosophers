#include <semaphore.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    sem_unlink("/forks");
    sem_unlink("/print");
	
	sem_t *forks = sem_open("/forks", O_CREAT, 0644, 5);
    sem_t *print = sem_open("/print", O_CREAT, 0644, 1);
	struct timeval	tv;

    for (int i = 0; i < 5; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            sem_wait(forks);
            sem_wait(forks);

			gettimeofday(&tv, NULL);
            sem_wait(print);
            printf("Philosopher %d is eating, sec: %ld, usec: %ld\n", i+1, tv.tv_sec, tv.tv_usec);
            sem_post(print);

            usleep(2000000);
            sem_post(forks);
            sem_post(forks);
            _exit(0);
        }
    }

    for (int i = 0; i < 5; i++)
        waitpid(-1, NULL, 0);

    sem_close(forks);
    sem_close(print);
    // sem_unlink("/forks");
    // sem_unlink("/print");

    return 0;
}
