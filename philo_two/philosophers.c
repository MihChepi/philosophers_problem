#include "philo_two.h"

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
	sem_wait(ph->communist);
	sem_wait(ph->fork_right);
	if (!ph->params->end)
		printf("%llu %d has taken a fork\n",
			current_time(ph->params), ph->num + 1);
	sem_wait(ph->fork_left);
	if (!ph->params->end)
		printf("%llu %d has taken a fork\n",
			current_time(ph->params), ph->num + 1);
	ph->start_eat = current_time(ph->params);
	if (!ph->params->end)
		printf("%llu %d is eating\n", ph->start_eat, ph->num + 1);
	ft_usleep(ph->params->time_to_eat);
	well_fed(ph);
	sem_post(ph->fork_right);
	sem_post(ph->fork_left);
	if (!ph->params->end)
		printf("%llu %d is sleeping\n", current_time(ph->params), ph->num + 1);
	ft_usleep(ph->params->time_to_sleep);
	if (!ph->params->end)
		printf("%llu %d is thinking\n", current_time(ph->params), ph->num + 1);
}

void	*main_pthread(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)(arg);
	ph->eats = ph->params->eats;
	ph->num_eat = 0;
	ph->start_eat = 0;
	sem_wait(ph->params->start);
	sem_post(ph->params->start);
	while (!ph->params->end)
		ph_cycle(ph);
	sem_post(ph->fork_left);
	sem_post(ph->fork_right);
	return (0);
}
