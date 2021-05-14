#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

struct s_ph;

typedef struct s_params
{
	pthread_mutex_t		*fork;
	pthread_mutex_t		start[1];
	struct timeval		t;
	unsigned long long	start_t;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_ph;
	int					eats;
	int					well_fed;
	int					dead;
	bool 				flag_eats;
	bool 				end;
	pthread_t			*threads;
	struct s_ph			*ph;
}				t_params;

typedef struct s_ph
{
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		*fork_right;
	pthread_mutex_t		communist;
	t_params			*params;
	unsigned long long	start_eat;
	int 				num;
	int 				num_eat;
	bool				dead;
	int 				eats;
}				t_ph;

void 				ft_usleep(unsigned long long t);
int					init_params(char **argv, t_params *p);
int					wrong_args(void);
unsigned long long	current_time(t_params *par);
void				*main_pthread(void *arg);
void				*communist(void *arg);
void				*stream_of_deaths(void *arg);

#endif