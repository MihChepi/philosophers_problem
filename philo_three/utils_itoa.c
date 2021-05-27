#include "philo_three.h"

char	*ft_maxmin(void)
{
	char	*s;
	int		n;
	int		i;

	i = 11;
	n = 214748364;
	s = (char *)malloc(12);
	if (!s)
		return (0);
	s[i--] = '\0';
	s[i--] = '8';
	while (n > 0)
	{
		s[i--] = n % 10 + 48;
		n = n / 10;
	}
	s[i] = '-';
	return (s);
}

char	*ft_zero(void)
{
	char	*s;

	s = (char *)malloc(2);
	if (!s)
		return (0);
	s[0] = '0';
	s[1] = '\0';
	return (s);
}

char	*ft_number(int n, int i, int tmp)
{
	char	*s;

	s = (char *)malloc(i + 1);
	if (!s)
		return (0);
	s[i--] = '\0';
	while (n > 0)
	{
		s[i--] = n % 10 + 48;
		n = n / 10;
	}
	if (tmp == -1)
		s[i] = '-';
	return (s);
}

char	*my_itoa(int n)
{
	char	*s;
	int		tmp;
	int		i;

	i = 0;
	tmp = n;
	if (n == 0)
		return (ft_zero());
	while (tmp != 0)
	{
		tmp /= 10;
		i++;
	}
	if (n == -2147483648)
		return (ft_maxmin());
	if (n < 0)
	{
		i++;
		tmp = -1;
		n = n * tmp;
	}
	s = ft_number(n, i, tmp);
	if (!s)
		return (0);
	return (s);
}
