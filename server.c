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

static int	signal_to_bit(int signal)
{
	if (signal == SIGUSR1)
		return (0);
	else if (signal == SIGUSR2)
		return (1);
	return (-1);
}

static void	print_char(char c)
{
	if (c == '\0')
		write(1, "\nMessage received.\n", 20);
	else
		write(1, &c, 1);
}

static void	add_bit(int bit)
{
	static char	g_data = 0;
	static int	shift = 7;

	g_data |= (bit << shift);
	shift--;
	if (shift < 0)
	{
		print_char(g_data);
		g_data = 0;
		shift = 7;
	}
}

static void	take_signal(int signal, siginfo_t *info, void *ucontext)
{
	int	bit;

	(void)ucontext;
	bit = signal_to_bit(signal);
	if (bit == -1)
		return ;
	add_bit(bit);
	if (info != NULL && info->si_pid != 0)
		kill(info->si_pid, SIGUSR1);
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
	while (1)
		pause();
}
