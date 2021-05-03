/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khermann <khermann@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 13:30:13 by khermann          #+#    #+#             */
/*   Updated: 2021/04/27 19:19:12 by khermann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
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

int	wrong_args(void)
{
	printf("Wrong number of args\n");
	return (1);
}

int init_params(char **argv, t_params *p)
{
	p->ph = ft_atoi(argv[1]);
	p->time_to_die = ft_atoi(argv[2]);
	p->time_to_eat = ft_atoi(argv[3]);
	p->time_to_sleep = ft_atoi(argv[4]);
	p->eats = -1;
	if (argv[5] != NULL)
		p->eats = ft_atoi(argv[5]);
	printf("num_of_ph = %d\ntime_to_die = %d\n time_to_eat %d\n time_to_sleep = %d\n eats = %d \n",
		p->ph, p->time_to_die, p->time_to_eat, p->time_to_sleep, p->eats);
	if (p->ph < 1 ||  p->time_to_die < 1 || p->time_to_eat < 1 ||
			p->time_to_sleep < 1)
		return (1);
	return (0);
}

void	print_status(t_ph *ph)
{
//	◦ timestamp_in_ms %d has taken a fork
//	◦ timestamp_in_ms %d is eating
//	◦ timestamp_in_ms %d is sleeping
//	◦ timestamp_in_ms %d is thinking
//	◦ timestamp_in_ms %d died
}

unsigned long long time_time(t_params *par)
{
	unsigned long long time;

	gettimeofday(&par->t, NULL);
	time = par->t.tv_sec * 1000 + par->t.tv_usec / 1000;
	time = time - par->start_t;
	return (time);
}


void	*main_pthread(void *arg)
{
	t_ph	*ph;
	ph = (t_ph*)(arg);
	(void)ph;
	unsigned long long time;

	while(ph->params->eats)
	{
		if (ph->params->ready_to_eat < ph->params->ph - 2)
		{
			++ph->params->ready_to_eat;
			pthread_mutex_lock(ph->fork_left);
			printf("%llu %d has taken a fork left\n", time_time(ph->params), ph->num);
			pthread_mutex_lock(ph->fork_right);
			ph->params->ready_to_eat--;
			printf("%llu %d has taken a fork right\n",time_time(ph->params), ph->num);

			printf("%llu %d is eating\n", time_time(ph->params), ph->num);
			usleep(ph->params->time_to_eat);


			pthread_mutex_unlock(ph->fork_left);
			pthread_mutex_unlock(ph->fork_right);

			printf("%llu %d is sleeping\n", time_time(ph->params), ph->num);
			usleep(ph->params->time_to_sleep);

			printf("%llu %d is thinking\n", time_time(ph->params), ph->num);
			if (ph->params->eats > 0)
				ph->params->eats--;
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_params		par;


	setbuf(stdout, 0);
	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return(wrong_args());


	t_ph				ph[par.ph];
	pthread_t			thread[par.ph]; // инициализируем потоки

	par.fork = malloc(sizeof(pthread_mutex_t) * par.ph); // инициализируем мьютексы

	int					i;
	i = -1;
	while(par.ph != ++i)
		pthread_mutex_init(&par.fork[i], NULL); // создаем мьютексы - вилки

	i = -1;
	while(par.ph != ++i + 1)
	{
		ph[i].fork_left = &par.fork[i];
		ph[i].fork_right = &par.fork[i + 1];
		ph[i].num = i;
		ph[i].params = &par;
	}
	ph[i].fork_left = &par.fork[i];
	ph[i].fork_right = &par.fork[0];
	ph[i].num = i;
	ph[i].params = &par;

	par.ready_to_eat = 0; //инициализируем колличество едящих философов
	gettimeofday(&par.t, NULL);
	par.start_t = par.t.tv_sec * 1000 + par.t.tv_usec / 1000;

	i = -1;
	while(par.ph != ++i)
		pthread_create(&thread[i], NULL, main_pthread, (void *)&ph[i]); // создаем потоки - философов

	i = -1;
	while(par.ph != ++i)
		pthread_join(thread[i], NULL);
}
