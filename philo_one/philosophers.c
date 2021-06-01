#include "philo_one.h"

void	well_fed(t_ph *ph)
{
	if (ph->eats > 0)
		--ph->eats;
	if (ph->eats == 0)
	{
		++ph->params->well_fed;
		--ph->eats;
	}
}

void	ph_cycle(t_ph *ph)
{
	pthread_mutex_lock(&ph->communist);
	pthread_mutex_lock(ph->fork_right);
	if (!ph->params->end)
		printf("%llu %d has taken a fork\n",
			current_time(ph->params), ph->num + 1);
	pthread_mutex_lock(ph->fork_left);
	if (!ph->params->end)
		printf("%llu %d has taken a fork\n",
			current_time(ph->params), ph->num + 1);
	ph->start_eat = current_time(ph->params);
	if (!ph->params->end)
		printf("%llu %d is eating\n", ph->start_eat, ph->num + 1);
	ft_usleep(ph->params->time_to_eat);
	well_fed(ph);
	pthread_mutex_unlock(ph->fork_right);
	pthread_mutex_unlock(ph->fork_left);
	if (!ph->params->end)
		printf("%llu %d is sleeping\n", current_time(ph->params), ph->num + 1);
	ft_usleep(ph->params->time_to_sleep);
	if (!ph->params->end)
		printf("%llu %d is thinking\n", current_time(ph->params), ph->num + 1);
}

void	*main_pthread(void *arg)
{
	t_ph		*ph;

	ph = (t_ph *)(arg);
	ph->eats = ph->params->eats;
	ph->num_eat = 0;
	ph->start_eat = 0;
	pthread_mutex_lock(ph->params->start);
	pthread_mutex_unlock(ph->params->start);
	while (!ph->params->end)
		ph_cycle(ph);
	pthread_mutex_unlock(ph->fork_left);
	pthread_mutex_unlock(ph->fork_right);
	return (0);
}
