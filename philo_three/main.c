/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khermann <khermann@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 13:30:13 by khermann          #+#    #+#             */
/*   Updated: 2021/05/28 12:18:14 by khermann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	create_fork(t_params *par, t_ph *ph)
{
	int		i;

	i = -1;
	par->sem_name = malloc(par->num_ph);
	while (par->num_ph != ++i)
	{
		par->fork[i] = malloc(sizeof(sem_t));
		par->sem_name[i] = create_sem_name_fork(i);
		sem_unlink(par->sem_name[i]);
		par->fork[i] = sem_open(par->sem_name[i], O_CREAT, 0666, 1);
		free(par->sem_name[i]);
		par->sem_name[i] = NULL;
	}
	i = -1;
	while (par->num_ph != ++i + 1)
	{
		ph[i].fork_left_hand = par->fork[i];
		ph[i].fork_right_hand = par->fork[i + 1];
		ph[i].num = i;
		ph[i].params = par;
	}
	ph[i].fork_left_hand = par->fork[i];
	ph[i].fork_right_hand = par->fork[0];
	ph[i].num = i;
	ph[i].params = par;
}

void	create_ph(t_params *par, t_ph *ph)
{
	int			i;

	par->death = malloc(sizeof (pthread_t) * par->num_ph);
	par->main = malloc(sizeof (pthread_t) * par->num_ph);
	i = -1;
	while (par->num_ph != ++i)
	{
		par->pid[i] = fork();
		if (par->pid[i] == 0)
		{
			pthread_create(&par->death[i], NULL, stream_of_deaths,
				  (void *) &ph[i]);
			pthread_detach(par->death[i]);
			pthread_create(&par->main[i], NULL, main_pthread, (void *) &ph[i]);
			pthread_detach(par->main[i]);
			sem_wait(ph->params->stop);
			sem_post(ph->params->stop);
			exit(0);
		}
	}
}

void	wait_ph(t_params *par)
{
	int			stat;
	int			i;

	i = -1;
	waitpid(0, &stat, WUNTRACED);
	if (stat == 0)
	{
		kill(par->waiter, SIGKILL);
	}
	if (stat == 256)
	{
		while (++i < par->num_ph)
			waitpid(par->pid[i], &stat, 0);
		kill(par->waiter, SIGKILL);
	}
}

void	ph_init(t_params *par)
{
	par->end = 0;
	par->well_fed = 0;
	par->ph = malloc(sizeof(t_ph) * par->num_ph);
	par->fork = malloc(sizeof(sem_t *) * par->num_ph);
	par->start = malloc(sizeof (sem_t));
	par->stop = malloc(sizeof (sem_t));
	par->pid = malloc((sizeof (pid_t)) * par->num_ph);
	create_fork(par, par->ph);
	create_sem_start(par);
	create_sem_stop(par);
	init_sem_communist(par);
	par->waiter = fork();
	if (par->waiter == 0)
	{
		communist(par);
		exit (0);
	}
	start_time(par);
	create_ph(par, par->ph);
	sem_post(par->start);
	wait_ph(par);
}

int	main(int argc, char **argv)
{
	t_params	par;

	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return (wrong_args());
	ph_init(&par);
	ft_usleep(500000);
	return (0);
}
