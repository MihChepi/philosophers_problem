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

int	create_ph(t_params *par)
{
	int	i;

	i = -1;
	while (par->num_ph != ++i)
	{
		if (pthread_create(&par->threads[i],
				NULL, main_pthread, (void *) &par->ph[i]))
			return (1);
	}
	i = -1;
	while (par->num_ph != ++i)
		pthread_detach(par->threads[i]);
	return (0);
}

int	ph_init(t_params *par)
{
	pthread_t	waiter;

	par->ph = malloc(sizeof(t_ph) * par->num_ph);
	par->threads = malloc(sizeof(pthread_t) * (par->num_ph));
	par->fork = malloc(sizeof(pthread_mutex_t) * par->num_ph);
	par->well_fed = 0;
	par->end = 0;
	if (!par->ph || !par->threads || !par->fork)
		return (1);
	create_fork(par, par->ph);
	pthread_create(&waiter, NULL, communist, (void *) par);
	start_time(par);
	if (create_ph(par))
		return (1);
	pthread_create(&par->death, NULL, stream_of_deaths, (void *) par);
	pthread_detach(waiter);
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	par;

	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return (wrong_args());
	if (ph_init(&par))
		return (fail_func());
	pthread_mutex_unlock(par.start);
	pthread_join(par.death, NULL);
	ft_usleep(10000);
	return (0);
}
