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
	char	**sem_name;

	i = -1;
	sem_name = malloc(par->num_ph);
	while (par->num_ph != ++i)
	{
		sem_name[i] = create_sem_name_fork(i);
		sem_unlink(sem_name[i]);
		par->fork[i] = sem_open(sem_name[i], O_CREAT, 0666, 1);
		free(sem_name[i]);
	}
	free(sem_name);
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

void	create_ph(t_params *par, t_ph *ph, pthread_t *thread)
{
	int	i;

	i = -1;
	while (par->num_ph != ++i)
	{
		pthread_create(&thread[i], NULL, main_pthread, (void *) &ph[i]);
		pthread_detach(thread[i]);
	}
}

void	ph_init(t_params *par)
{
	pthread_t	waiter;
	t_ph		*ph;
	pthread_t	*thread;

	par->end = 0;
	ph = malloc(sizeof(t_ph) * par->num_ph);
	thread = malloc(sizeof(pthread_t) * (par->num_ph));
	par->threads = thread;
	par->fork = malloc(sizeof(sem_t *) * par->num_ph);
	par->well_fed = 0;
	par->ph = ph;
	create_fork(par, ph);
	pthread_create(&waiter, NULL, communist, (void *) par);
	usleep(10000);
	start_time(par);
	create_ph(par, ph, thread);
	pthread_create(&par->death, NULL, stream_of_deaths, (void *) par);
	pthread_detach(waiter);
}

int	main(int argc, char **argv)
{
	t_params	par;

	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return (wrong_args());
	create_sem_start(&par);
	ph_init(&par);
	sem_post(par.start);
	pthread_join(par.death, NULL);
	ft_usleep(10000);
	return (0);
}
