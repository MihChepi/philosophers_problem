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
//		p->ph, p->time_to_die, p->time_to_eat, p->time_to_sleep, p->eats);
	if (p->ph < 1 ||  p->time_to_die < 1 || p->time_to_eat < 1 ||
			p->time_to_sleep < 1)
		return (1);
	return (0);
}

unsigned long long current_time(t_params *par)
{
	unsigned long long time;

	gettimeofday(&par->t, NULL);
	time = par->t.tv_sec * 1000 + par->t.tv_usec / 1000;
	time = time - par->start_t;
	return (time);
}

void	*main_pthread_right(void *arg)
{
	t_ph	*ph;
	int 	eats;

	eats = ph->params->eats;
	ph = (t_ph*)(arg);
	ph->start_eat = 0;
	while(eats)
	{
		if (ph->params->dead == 1)
			break ;
		pthread_mutex_lock(ph->fork_right);
		printf("%llu %d has taken a fork right\n",current_time(ph->params), ph->num + 1);
		pthread_mutex_lock(ph->fork_left);
		printf("%llu %d has taken a fork left\n", current_time(ph->params), ph->num + 1);
		if (current_time(ph->params) - ph->start_eat >= ph->params->time_to_die)
		{
			ph->params->dead = 1;
			printf("Philosoph %d is DEAD %llu\n", ph->num + 1, current_time(ph->params) - ph->start_eat);
			break ;
		}
		ph->start_eat = current_time(ph->params);
		printf("%llu %d is eating\n", ph->start_eat, ph->num + 1);
		usleep(ph->params->time_to_eat);
		if (eats > 0)
			--eats;
		pthread_mutex_unlock(ph->fork_right);
		pthread_mutex_unlock(ph->fork_left);
		printf("%llu %d is sleeping\n", current_time(ph->params), ph->num + 1);
		usleep(ph->params->time_to_sleep);
		printf("%llu %d is thinking\n", current_time(ph->params), ph->num + 1);
	}
	ph->params->dead = 1;
	return (0);
}

void	*main_pthread_left(void *arg)
{
	t_ph	*ph;
	int 	eats;

	eats = ph->params->eats;
	ph = (t_ph*)(arg);
	ph->start_eat = 0;
	while(eats)
	{
		if (ph->params->dead == 1)
			break ;
		pthread_mutex_lock(ph->fork_left);
		printf("%llu %d has taken a fork left\n", current_time(ph->params), ph->num + 1);
		pthread_mutex_lock(ph->fork_right);
		printf("%llu %d has taken a fork right\n",current_time(ph->params), ph->num + 1);
		if (current_time(ph->params) - ph->start_eat >= ph->params->time_to_die)
		{
			ph->params->dead = 1;
			printf("Philosoph %d is DEAD %llu\n", ph->num + 1, current_time(ph->params) - ph->start_eat);
			break ;
		}
		ph->start_eat = current_time(ph->params);
		printf("%llu %d is eating\n", ph->start_eat, ph->num + 1);
		usleep(ph->params->time_to_eat);
		if (eats > 0)
			--eats;
		pthread_mutex_unlock(ph->fork_left);
		pthread_mutex_unlock(ph->fork_right);
		printf("%llu %d is sleeping\n", current_time(ph->params), ph->num + 1);
		usleep(ph->params->time_to_sleep);
		printf("%llu %d is thinking\n", current_time(ph->params), ph->num + 1);
	}
	ph->params->dead = 1;
	return (0);
}

int	main(int argc, char **argv)
{
	t_params		par;

//	setbuf(stdout, 0);
	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return(wrong_args());


	t_ph				*ph;
	pthread_t			thread[par.ph + 1]; // инициализируем потоки

	ph = malloc(sizeof(t_ph) * par.ph);
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

	gettimeofday(&par.t, NULL);
	par.start_t = par.t.tv_sec * 1000 + par.t.tv_usec / 1000;  //начало отсчета
	ph->params->dead = 0;

	i = -1;
	while(par.ph != ++i )
	{
		if (i / 2 == 0)
			pthread_create(&thread[i], NULL, main_pthread_left, (void *) &ph[i]); // создаем потоки - философов
		else
			pthread_create(&thread[i], NULL, main_pthread_right, (void *) &ph[i]);
	}

//	i = 0;
//	while (par.dead == 0)
//	{
//		usleep(5000);
//		if (current_time(ph[i].params) - ph[i].start_eat >= ph[i].params->time_to_die)
//		{
//			ph->params->dead = 1;
//			printf("Philosoph is DEAD %llu\n", current_time(ph[i].params) - ph[i].start_eat);
//			break;
//		}
//		i++;
//		if (i == par.ph - 1)
//			i = 0;
//	}

	i = 0;
	while(par.ph - 1 != i)
	{
		usleep(5000);
		pthread_join(thread[i], NULL);
		i++;
	}
	return (0);
}
