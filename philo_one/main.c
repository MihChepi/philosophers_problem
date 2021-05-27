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

void	create_mutex_start(t_params *par)
{
	pthread_mutex_init(par->start, NULL);
	pthread_mutex_lock(par->start);
}

void	create_fork(t_params *par, t_ph *ph)
{
	int	i;

	i = -1;
	while (par->num_ph != ++i)
		pthread_mutex_init(&par->fork[i], NULL);
	i = -1;
	while (par->num_ph != ++i + 1)
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

void	create_ph(t_params *par, t_ph *ph, pthread_t *thread)
{
	int	i;

	i = -1;
	while (par->num_ph != ++i)
		pthread_create(&thread[i], NULL, main_pthread, (void *) &ph[i]);
}

void	ph_init(t_params *par)
{
	pthread_t	waiter;
	pthread_t	death;
	t_ph		*ph;
	pthread_t	*thread;

	par->end = 0;
	ph = malloc(sizeof(t_ph) * par->num_ph);
	thread = malloc(sizeof(pthread_t) * (par->num_ph));
	par->threads = thread;
	par->fork = malloc(sizeof(pthread_mutex_t) * par->num_ph);
	par->well_fed = 0;
	par->ph = ph;
	create_fork(par, ph);
	pthread_create(&waiter, NULL, communist, (void *) par);
	start_time(par);
	create_ph(par, ph, thread);
	pthread_create(&death, NULL, stream_of_deaths, (void *) par);
	pthread_detach(waiter);
	pthread_detach(death);
}

int	main(int argc, char **argv)
{
	t_params	par;
	int			i;

	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return (wrong_args());
	par.end_all = 0;
	ph_init(&par);
	pthread_mutex_unlock(par.start);
	while (par.end == 0)
		usleep(1);
	i = 0;
	while (i <= par.num_ph)
		pthread_join(par.threads[i++], NULL);
	par.end_all = 1;
	return (0);
}
