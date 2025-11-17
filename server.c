/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 19:28:46 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/17 18:08:18 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <stdio.h>

static void	take_signal(int signal)
{
	static char	g_data = 0;
	static int	shift = 7;
	int			bit;

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
		if (g_data == '\n')
			write(1, "\nMessage received.", 19);
		write(1, &g_data, 1);
		g_data = 0;
		shift = 7;
	}
}

int	main(void)
{
	write(1, "Server running on PID: ", 22);
	ft_putnbr(getpid());
	write(1, "\n", 1);
	signal(SIGUSR1, take_signal);
	signal(SIGUSR2, take_signal);
	write(1, "\n", 1);
	while (1)
		pause();
}
