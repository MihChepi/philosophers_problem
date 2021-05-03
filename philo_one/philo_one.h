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
	int					ready_to_eat;
	int					ph;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					eats;
	unsigned long long	start_t;
	struct timeval		t;
}				t_params;

typedef struct s_ph
{
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	t_params		*params;
	int 			num;
	bool			dead;
}				t_ph;
