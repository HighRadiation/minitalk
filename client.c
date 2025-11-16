/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:27:59 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/16 22:06:59 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	g_server_pid;

static void	send_to_server(char bit)
{
	if (bit == 0)
		kill(g_server_pid, SIGUSR1);
	else
		kill(g_server_pid, SIGUSR2);
	usleep(200);
}

// bit kaydırma yaparak mesajı işliyor,
static void	procces_message(char *str)
{
	int	i;
	int	bit;
	int	shift;

	i = 0;
	shift = 7;
	while (str[i])
	{
		while (shift >= 0)
		{
			bit = (str[i] >> shift) & 1;
			send_to_server(bit);
			shift--;
		}
		i++;
	}
	shift = 7;
	while (shift >= 0)
	{
		bit = ('\0' >> shift) & 1;
		send_to_server(bit);
		shift--;
	}
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		write(1, "\n", 1);
		return (0);
	}
	g_server_pid = atoi(argv[1]);
	procces_message(argv[2]);
}
