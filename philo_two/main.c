/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khermann <khermann@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 13:30:13 by khermann          #+#    #+#             */
/*   Updated: 2021/06/02 16:34:55 by khermann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	create_sem_start(t_params *par)
{
	sem_unlink("/start");
	par->start = sem_open("/start", O_CREAT, 0666, 1);
	sem_wait(par->start);
}

void	create_fork(t_params *par, t_ph *ph)
{
	int		i;

	i = -1;
	par->sem_name_fork = malloc(sizeof (char *) * par->num_ph);
	while (par->num_ph != ++i)
	{
		par->sem_name_fork[i] = create_sem_name_fork(i);
		sem_unlink(par->sem_name_fork[i]);
		par->fork[i] = sem_open(par->sem_name_fork[i], O_CREAT, 0666, 1);
		free(par->sem_name_fork[i]);
		par->sem_name_fork[i] = NULL;
	}
	i = -1;
	while (par->num_ph != ++i + 1)
	{
		ph[i].fork_left = par->fork[i];
		ph[i].fork_right = par->fork[i + 1];
		ph[i].num = i;
		ph[i].params = par;
	}
	ph[i].fork_left = par->fork[i];
	ph[i].fork_right = par->fork[0];
	ph[i].num = i;
	ph[i].params = par;
}

int	create_ph(t_params *par, t_ph *ph)
{
	int	i;

	i = -1;
	while (par->num_ph != ++i)
	{
		if (pthread_create(&par->threads[i],
				NULL, main_pthread, (void *) &ph[i]))
			return (1);
		pthread_detach(par->threads[i]);
	}
	return (0);
}

int	ph_init(t_params *par)
{
	pthread_t	waiter;

	par->ph = malloc(sizeof(t_ph) * par->num_ph);
	par->threads = malloc(sizeof(pthread_t) * (par->num_ph));
	par->fork = malloc(sizeof(sem_t *) * par->num_ph);
	par->well_fed = 0;
	par->end = 0;
	create_fork(par, par->ph);
	if (pthread_create(&waiter, NULL, communist, (void *) par))
		return (1);
	create_ph(par, par->ph);
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
	create_sem_start(&par);
	if (ph_init(&par))
		return (1);
	start_time(&par);
	sem_post(par.start);
	pthread_join(par.death, NULL);
	ft_usleep(100000);
	return (0);
}
