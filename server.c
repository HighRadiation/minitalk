/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 19:28:46 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/16 22:02:18 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	g_data;

static void	take_signal(int signal)
{
	int			bit;
	static int	shift = 7;

	if (signal == SIGUSR1)
		bit = 0;
	else if (signal == SIGUSR2)
		bit = 1;
	else
		return ;
	g_data |= (bit << shift);
	shift--;
	if (shift < 0)
	{
		if (g_data == '\0')
			write(1, "\n", 1);
		else
			write(1, &g_data, 1);
		g_data = 0;
		shift = 7;
	}
}

int	main(void)
{
	write(1, "Server runnig on PID: ", 22);
	ft_putnbr(getpid());
	write(1, "\n", 1);
	signal(SIGUSR1, take_signal);
	signal(SIGUSR2, take_signal);
	while (1)
		pause();
}
