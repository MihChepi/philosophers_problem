#include "philo_one.h"

static unsigned long long current_time(void)
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
	start = current_time();
	now = start;
//	printf("start = %llu\n", start);
//	printf("now = %llu\n", now);
//	printf("t = %llu ", t);
	while((now - start) < t)
	{
		usleep(1);
		now = current_time();
	}
}