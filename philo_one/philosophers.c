#include "philo_one.h"

void	*main_pthread(void *arg)
{
	t_ph	*ph;
	int		eats;

	usleep(10000);
	ph = (t_ph*)(arg);
	eats = ph->params->eats;
	ph->num_eat = 0;
	ph->start_eat = 0;
	while(eats)
	{
		if (ph->params->dead == 1)
			break ;
		pthread_mutex_lock(&ph->communist);
		pthread_mutex_lock(ph->fork_right);
		printf("%llu %d has taken a fork\n",current_time(ph->params), ph->num + 1);
		pthread_mutex_lock(ph->fork_left);
		printf("%llu %d has taken a fork\n", current_time(ph->params), ph->num + 1);
		ph->start_eat = current_time(ph->params);
		printf("%llu %d is eating\n", ph->start_eat, ph->num + 1);
		ft_usleep(ph->params->time_to_eat);
		++ph->num_eat;
		if (eats > 0)
			--eats;
		pthread_mutex_unlock(ph->fork_right);
		pthread_mutex_unlock(ph->fork_left);
		printf("%llu %d is sleeping\n", current_time(ph->params), ph->num + 1);
		ft_usleep(ph->params->time_to_sleep);
		printf("%llu %d is thinking\n", current_time(ph->params), ph->num + 1);
	}
	pthread_mutex_unlock(ph->fork_left);
	pthread_mutex_unlock(ph->fork_right);
	if (!(ph->params->eats > 0))
		ph->params->dead = 1;
	++ph->params->well_fed;
	return (0);
}