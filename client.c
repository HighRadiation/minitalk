/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:27:59 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/30 16:53:38 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

// Global variable, volatile = Derleyici optimizasyonunu engeller, değerin her zaman RAM'den okunmasını sağlar.
static volatile sig_atomic_t	g_received = 0;

// onay işleme //
static void	ack_handler(int sig)
{
	(void)sig;
	// global değişkeni bir yapar ve  main loop bekleme modundan çıkabileceğini anlar9
	g_received = 1;
}

// server'a gönderme işlemi
static void	send_to_server(int server_pid, char bit)
{
	sigset_t	mask;
	sigset_t	old_mask;

	sigemptyset(&mask); // maske setini boşalt /temizle
	sigaddset(&mask, SIGUSR1); // kümeye sadece sıgusr1 ekle
	sigprocmask(SIG_BLOCK, &mask, &old_mask); // sigusr1 sinyalini bloke et

	g_received = 0; // bayrağı sıfırla
	if (bit == 0)
		kill(server_pid, SIGUSR1); // bit'in 0 ve 1 olma durumu var 0 ise SIGUSR1 ile 0 gönder
	else
		kill(server_pid, SIGUSR2); // değilse 1 gönder

	// // Sinyal kaybını (race condition) önlemek için atomik olarak maskeyi açar ve bekler. atomik: bölünemez işlem
	while (!g_received)
		sigsuspend(&old_mask);

	sigprocmask(SIG_UNBLOCK, &mask, NULL); // Fonksiyondan çıkmadan önce blokladığımız SIGUSR1 sinyalini tekrar serbest bırakıyoruz
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
	int			server_pid;

	if (argc != 3)
	{
		write(2, "Error: ./client [server_pid] [text]\n", 36);
		return (1);
	}
	server_pid = check_overflow(argv[1]);
	if (server_pid == 0)
		return (1);
	sa.sa_handler = ack_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	procces_message(server_pid, argv[2]);
	return (0);
}
