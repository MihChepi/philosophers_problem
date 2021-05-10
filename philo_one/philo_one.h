#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct s_params
{
	pthread_mutex_t		*fork;
	struct timeval		t;
	unsigned long long	start_t;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					ph;
	int					eats;
	bool 				dead;
	bool 				flag_eats;
}				t_params;

typedef struct s_ph
{
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		*fork_right;
	t_params			*params;
	unsigned long long	start_eat;
	int 				num;
	bool				dead;
}				t_ph;
