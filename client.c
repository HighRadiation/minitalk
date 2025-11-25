/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:27:59 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/19 17:03:44 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile sig_atomic_t	g_received = 0;

static void	ack_handler(int sig)
{
	(void)sig;
	g_received = 1;
}

static void	send_to_server(int server_pid, char bit)
{
	g_received = 0;
	if (bit == 0)
		kill(server_pid, SIGUSR1);
	else
		kill(server_pid, SIGUSR2);
	while (!g_received)
		pause();
}

static void	procces_message(int server_pid, char *str)
{
	int	i;
	int	bit;
	int	shift;

	i = 0;
	while (str[i])
	{
		shift = 7;
		while (shift >= 0)
		{
			bit = (str[i] >> shift) & 1;
			send_to_server(server_pid, bit);
			shift--;
		}
		i++;
	}
	shift = 7;
	while (shift >= 0)
	{
		bit = ('\0' >> shift) & 1;
		send_to_server(server_pid, bit);
		shift--;
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;

	if (argc != 3)
	{
		write(2, "Error: ./client [server_pid] [text]\n", 36);
		return (1);
	}
	sa.sa_handler = ack_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	procces_message(ft_atoi(argv[1]), argv[2]);
	return (0);
}
