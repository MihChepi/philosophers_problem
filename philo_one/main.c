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


void ph_init(t_params *par)
{
	t_ph				*ph;
	pthread_t			thread[par->num_ph + 1]; // инициализируем потоки
	pthread_t			waiter[1];

	ph = malloc(sizeof(t_ph) * par->num_ph);
	par->fork = malloc(sizeof(pthread_mutex_t) * par->num_ph); // инициализируем мьютексы

	par->well_fed = 0;
	par->ph = ph;
	int					i;
	i = -1;
	while(par->num_ph != ++i)
		pthread_mutex_init(&par->fork[i], NULL); // создаем мьютексы - вилки

	i = -1;
	while(par->num_ph != ++i + 1)
	{
		ph[i].fork_left = &par->fork[i];
		ph[i].fork_right = &par->fork[i + 1];
		ph[i].num = i;
		ph[i].params = par;
	}
	ph[i].fork_left = &par->fork[i];
	ph[i].fork_right = &par->fork[0];
	ph[i].num = i;
	ph[i].params = par;

	pthread_create(waiter, NULL, communist, (void *) par);
	usleep(10000);

	gettimeofday(&par->t, NULL);
	par->start_t = par->t.tv_sec * 1000 + par->t.tv_usec / 1000; //начало отсчета
	ph->params->dead = 0;

	i = -1;
	while(par->num_ph != ++i)
	{
		pthread_create(&thread[i], NULL, main_pthread,
					   (void *) &ph[i]); // создаем потоки - философов
	}

	pthread_create(&thread[i], NULL, stream_of_deaths, (void *) par);
}


int	main(int argc, char **argv)
{
	t_params		par;

	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return (wrong_args());
	ph_init(&par);
//	t_ph				*ph;
//	pthread_t			thread[par.num_ph + 1]; // инициализируем потоки
//	pthread_t			waiter[1];
//
//	ph = malloc(sizeof(t_ph) * par.num_ph);
//	par.fork = malloc(sizeof(pthread_mutex_t) * par.num_ph); // инициализируем мьютексы
//
//	par.well_fed = 0;
//	par.ph = ph;
//	int					i;
//	i = -1;
//	while(par.num_ph != ++i)
//		pthread_mutex_init(&par.fork[i], NULL); // создаем мьютексы - вилки
//
//	i = -1;
//	while(par.num_ph != ++i + 1)
//	{
//		ph[i].fork_left = &par.fork[i];
//		ph[i].fork_right = &par.fork[i + 1];
//		ph[i].num = i;
//		ph[i].params = &par;
//	}
//	ph[i].fork_left = &par.fork[i];
//	ph[i].fork_right = &par.fork[0];
//	ph[i].num = i;
//	ph[i].params = &par;
//
//	pthread_create(waiter, NULL, communist, (void *) &par);
//	usleep(10000);
//
//	gettimeofday(&par.t, NULL);
//	par.start_t = par.t.tv_sec * 1000 + par.t.tv_usec / 1000; //начало отсчета
//	ph->params->dead = 0;
//
//	i = -1;
//	while(par.num_ph != ++i)
//	{
//			pthread_create(&thread[i], NULL, main_pthread,
//				  (void *) &ph[i]); // создаем потоки - философов
//	}
//
//	pthread_create(&thread[i], NULL, stream_of_deaths, (void *) &par);

	while(par.dead == 0 || par.well_fed == par.num_ph)
		usleep(1);
	return (0);
}

// сделать поток смертей ++
// сделать коммуниста +
// сделать мьютеккс на всё (сделал return в мэйне) +
