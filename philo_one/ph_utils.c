#include "philo_one.h"

void	start_time(t_params *par)
{
	gettimeofday(&par->t, NULL);
	par->start_t = par->t.tv_sec * 1000 + par->t.tv_usec / 1000;
}

unsigned long long	current_time(t_params *par)
{
	unsigned long long	time;

	gettimeofday(&par->t, NULL);
	time = par->t.tv_sec * 1000 + par->t.tv_usec / 1000;
	time = time - par->start_t;
	return (time);
}

static unsigned long long	current_get_time_from_start(void)
{
	unsigned long long		time;
	static struct timeval	t;

	gettimeofday(&t, NULL);
	time = t.tv_sec * 1000 + t.tv_usec / 1000;
	return (time);
}

void 	ft_usleep(unsigned long long t)
{
	unsigned long long	start;
	unsigned long long	now;

	t = t / 1000;
	start = current_get_time_from_start();
	now = start;
	while ((now - start) < t)
	{
		usleep(300);
		now = current_get_time_from_start();
	}
}

int	fail_func(void)
{
	printf("fail memory allocation\n");
	return (1);
}
