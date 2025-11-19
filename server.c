/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 19:28:46 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/19 17:20:18 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	take_signal(int signal, siginfo_t *info, void *ucontext)
{
	static char	g_data = 0;
	static int	shift = 7;
	int			bit;

	(void)info;
	(void)ucontext;
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
			write(1, "\nMessage received.", 19);
		write(1, &g_data, 1);
		g_data = 0;
		shift = 7;
	}
}

int	main(void)
{
	struct sigaction	var;

	write(1, "Server running on PID: ", 22);
	ft_putnbr(getpid());
	write(1, "\n", 1);
	sigemptyset(&var.sa_mask);
	sigaddset(&var.sa_mask, SIGUSR1);
	sigaddset(&var.sa_mask, SIGUSR2);
	var.sa_flags = SA_SIGINFO;
	var.sa_sigaction = take_signal;
	sigaction(SIGUSR1, &var, NULL);
	sigaction(SIGUSR2, &var, NULL);
	write(1, "\n", 1);
	while (1)
		pause();
}
