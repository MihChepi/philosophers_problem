#include "philo_two.h"

void	*stream_of_deaths(void *arg)
{
	t_params	*par;
	int			i;

	par = (t_params *)(arg);
	i = -1;
	sem_wait(par->start);
	sem_post(par->start);
	ft_usleep(1000);
	while (par->num_ph != ++i)
	{
		usleep(50);
		if ((int)(current_time(par->ph[i].params) - par->ph[i].start_eat)
			>= par->ph[i].params->time_to_die)
			break ;
		if (i == par->num_ph - 1)
			i = -1;
		if (par->well_fed >= par->num_ph)
			break ;
	}
	if (!(par->well_fed == par->num_ph))
		printf("%llu %d died\n", current_time(par), i + 1);
	par->end = 1;
	return (0);
}

char	*create_sem_name(int i)
{
	char	*str;
	char	*num;

	str = malloc(100);
	str[0] = '/';
	str[1] = 'c';
	str[2] = 'm';
	num = my_itoa(i);
	strlcat(str, num, 100);
	free(num);
	return (str);
}

void 	init_mutex_communist(t_params *par)
{
	int		i;

	par->sem_name_communist = malloc(sizeof (char *) * par->num_ph);
	i = -1;
	while (par->num_ph != ++i)
	{
		par->sem_name_communist[i] = create_sem_name(i);
		sem_unlink(par->sem_name_communist[i]);
		par->ph[i].communist = sem_open(par->sem_name_communist[i],
				O_CREAT, 0666, 1);
		free(par->sem_name_communist[i]);
	}
	i = -1;
	while (par->num_ph != ++i)
		sem_wait(par->ph[i].communist);
}

void 	communism(t_params *par)
{
	int	eats;
	int	can_eat;
	int	max_eat;
	int	i;

	max_eat = 1;
	if (par->num_ph > 1)
		max_eat = par->num_ph / 2;
	i = 0;
	sem_wait(par->start);
	sem_post(par->start);
	while (!par->end_all)
	{
		can_eat = i;
		eats = 0;
		while (eats < max_eat)
		{
			sem_post(par->ph[can_eat].communist);
			can_eat += 2;
			can_eat %= par->num_ph;
			++eats;
		}
		ft_usleep(par->time_to_eat);
		i = (i + 1) % par->num_ph;
	}
}

void	*communist(void *arg)
{
	t_params	*par;

	par = (t_params *)(arg);
	init_mutex_communist(par);
	communism(par);
	return (0);
}
