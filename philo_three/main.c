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

#include "philo_three.h"

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
	pthread_t	*death;

	death = malloc(sizeof (pthread_t) * par->num_ph);
	i = -1;
	while (par->num_ph != ++i)
	{
		par->pid[i] = fork();
		if (par->pid[i] == 0)
		{
			pthread_create(&death[i], NULL, stream_of_deaths, (void *) &ph[i]);
			pthread_detach(death[i]);
			main_pthread((void *) &ph[i]);
			if (ph->params->well_fed == ph->params->num_ph)
			{
				sem_post(par->dead);
				exit(0);
			}
			printf("exit--------------\n");
			sem_post(par->dead);
			exit(1);
		}
	}
}

void 	ph_status(t_params *par)
{
	sem_unlink("/dead");
	par->dead = sem_open("/dead", O_CREAT, 0666, 1);
	par->status = fork();
	if(par->status == 0)
	{
		sem_wait(par->dead);
		printf("first-----------\n");
		sem_wait(par->dead);
		printf("second----------\n");
		exit(1);
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
	par->fork = malloc(sizeof(sem_t *) * par->num_ph); //?????
	par->pid = malloc((sizeof (pid_t)) * par->num_ph);
	par->well_fed = 0;
	par->ph = ph;
	create_fork(par, ph);
	pthread_create(&waiter, NULL, communist, (void *) par);
	pthread_detach(waiter);
	start_time(par);


	ph_status(par);
	create_ph(par, ph);

	sem_post(par->start);

	int			stat;
	int			i;

	i = -1;

 	waitpid(par->status, &stat, WUNTRACED);
	printf("turn wiatpid--------- stat = % d\n", stat);
 	exit(0);


	if (stat == 0)
		while (++i < par->num_ph)
			wait(&par->pid[i]);
	if (stat == 1)
		while (++i < par->num_ph)
			kill(par->pid[i], SIGKILL);
}

int	main(int argc, char **argv)
{
	t_params	par;

	if (argc != 6 && argc != 5)
		return (wrong_args());
	if (init_params(argv, &par))
		return (wrong_args());
	par.end_all = 0;
	create_sem_start(&par);
	ph_init(&par);
//	while (par.end == 0)
//		usleep(1);
	par.end_all = 1;
	return (0);
}
