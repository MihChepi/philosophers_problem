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

void start_time(t_params *par)
{
	gettimeofday(&par->t, NULL);
	par->start_t = par->t.tv_sec * 1000 + par->t.tv_usec / 1000;
}
void create_mutex_start(t_params *par)
{
	pthread_mutex_init(par->start, NULL);
	pthread_mutex_lock(par->start);
}

void create_fork(t_params *par, t_ph *ph)
{
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
}

void create_ph(t_params *par, t_ph *ph, pthread_t *thread)
{
	int i;

	i = -1;
	while(par->num_ph != ++i)
		pthread_create(&thread[i], NULL, main_pthread, (void *) &ph[i]); // создаем потоки - философов
}

void ph_init(t_params *par)
{
	t_ph				*ph;
	pthread_t			*thread; // инициализируем потоки
	pthread_t			waiter[1];
	pthread_t			death[1];

	par->end = 0;
	ph = malloc(sizeof(t_ph) * par->num_ph);
	thread = malloc(sizeof(pthread_t) * (par->num_ph));
	par->threads = thread;
	par->fork = malloc(sizeof(pthread_mutex_t) * par->num_ph); // инициализируем мьютексы
	par->well_fed = 0;
	par->ph = ph;
	create_fork(par, ph);
	pthread_create(waiter, NULL, communist, (void *) par);
	usleep(10000);
	start_time(par);
	create_ph(par, ph, thread);
	pthread_create(death, NULL, stream_of_deaths, (void *) par);
}

int	main(int argc, char **argv)
{
	t_params		par;
	int 			i;

	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return (wrong_args());
	par.end_all = 0;
	ph_init(&par);
	pthread_mutex_unlock(par.start);
	while(par.end == 0) //&& par.well_fed == par.num_ph)
		usleep(1);

	i = 0;
	while(i <= par.num_ph)
		pthread_join(par.threads[i++], NULL);
	par.end_all = 1;
	return (0);
}

// сделать поток смертей ++
// сделать коммуниста +
// сделать мьютеккс на всё (сделал return в мэйне) +
// 5 800 200 200 3
