#include "philo_one.h"

void	*stream_of_deaths(void *arg)
{
	t_params	*par;
	int 		i;

	par = (t_params*)(arg);
	i = -1;
	while(par->num_ph != ++i)
	{
		usleep(100);
		if ((int)current_time(par->ph[i].params) - par->ph[i].start_eat >=
			par->ph[i].params->time_to_die)
			break;
		if (i == par->num_ph - 1)
			i = -1;
	}
	par->ph[i].params->dead = 1;
	printf("%llu %d died\n", current_time(par), i + 1);
	return (0);
}

void	*communist(void *arg)
{
	t_params	*par;
	int i;

	par = (t_params*)(arg);
	i = -1;
	while(par->num_ph != ++i)
		pthread_mutex_init(&par->ph[i].communist, NULL); // создаем мьютексы - вилки

	i = -1;
	while(par->num_ph != ++i)
		pthread_mutex_lock(&par->ph[i].communist);

	int eats;
	int can_eat;
	int max_eat;

	max_eat = 1;

	if (par->num_ph > 1)
		max_eat = par->num_ph / 2;
	i = 0;
	while(!par->dead)
	{
		can_eat = i;
		eats = 0;
		while(eats < max_eat)
		{
			pthread_mutex_unlock(&par->ph[can_eat].communist);
			can_eat += 2;
			can_eat %= par->num_ph;
			eats++;
		}
		ft_usleep(par->time_to_eat);
		i++;
		i %= par->num_ph;
	}
	return (0);
}