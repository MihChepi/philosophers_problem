#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <semaphore.h>
# include <signal.h>

struct					s_ph;

typedef struct s_params
{
	sem_t				**fork;
	sem_t				*stop;
	sem_t				*start;
	struct timeval		t;
	unsigned long long	start_t;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_ph;
	int					eats;
	int					well_fed;
	bool				flag_eats;
	bool				end;
	bool				end_all;
	pid_t				*pid;
	pid_t				waiter;
	pthread_t			*death;
	pthread_t			*main;
	char				**sem_name;
	struct s_ph			*ph;
}				t_params;

typedef struct s_ph
{
	sem_t				*fork_left_hand;
	sem_t				*fork_right_hand;
	sem_t				*communist;
	t_params			*params;
	unsigned long long	start_eat;
	int					num;
	int					num_eat;
	bool				dead;
	int					eats;
}				t_ph;

void				ft_usleep(unsigned long long t);
int					init_params(char **argv, t_params *p);
int					wrong_args(void);
unsigned long long	current_time(t_params *par);
void				start_time(t_params *par);
void				*main_pthread(void *arg);
void				*communist(void *arg);
void				init_sem_communist(t_params *par);
void				*stream_of_deaths(void *arg);
char				*create_sem_name_fork(int i);
char				*my_itoa(int i);
void				create_sem_stop(t_params *par);
void				create_sem_start(t_params *par);

#endif