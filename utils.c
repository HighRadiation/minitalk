/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 19:32:55 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/17 16:48:52 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	ft_strlen(const char *str)
{
	int len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

void	ft_putnbr(int nbr)
{
	char	*base;

	base = "0123456789";
	if (nbr >= 10)
		ft_putnbr(nbr / 10);
	write(1, &base[nbr % 10], 1);
}

int check_overflow(const char *str_pid)
{
	int	pid;

	if (ft_strlen(str_pid) > 10)
	{
		write (2, "Error: Invalid PID\n", 19);
		return (0);
	}
	pid = ft_atoi(str_pid);
	if (pid <= 0)
	{
		write(2, "Error: Invalid PID\n", 19);
		return (0);
	}
	return (pid);
}