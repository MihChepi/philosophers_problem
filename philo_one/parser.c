#include "philo_one.h"

static int	ft_atoi(const char *str)
{
	long long int	i;
	long long int	num;
	int				minus;

	i = 0;
	num = 0;
	minus = 1;
	while ((str[i] >= 7 && str[i] <= 13) || str[i] == ' ')
		i++;
	if ((str[i] >= '0' && str[i] <= '9') || str[i] == '+' || str[i] == '-')
		if (str[i] == '-' || str[i] == '+')
			if (str[i++] == '-')
				minus = -1;
	if (str[i] >= '0' && str[i] <= '9')
		while (str[i] >= '0' && str[i] <= '9')
			num = (10 * num + (str[i++] - 48));
	return (num * minus);
}

int init_params(char **argv, t_params *p)
{
	p->num_ph = ft_atoi(argv[1]);
	p->time_to_die = ft_atoi(argv[2]);
	p->time_to_eat = ft_atoi(argv[3]) * 1000;
	p->time_to_sleep = ft_atoi(argv[4]) * 1000;
	p->eats = -1;
	if (argv[5] != NULL)
		p->eats = ft_atoi(argv[5]);
	if (p->eats >= 0)
		p->flag_eats = 0;
	else
		p->flag_eats = 1;
//	printf("num_of_ph = %d\ntime_to_die = %d\n time_to_eat %d\n time_to_sleep = %d\n eats = %d \n",
//		p->num_ph, p->time_to_die, p->time_to_eat, p->time_to_sleep, p->eats);
	if (p->num_ph < 1 ||  p->time_to_die < 1 || p->time_to_eat < 1 ||
		p->time_to_sleep < 1)
		return (1);
	return (0);
}

int	wrong_args(void)
{
	printf("Wrong number of args\n");
	return (1);
}
