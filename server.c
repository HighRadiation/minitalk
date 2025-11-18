/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 19:28:46 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/18 22:22:45 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void take_signal(int signal, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	static char g_data = 0;
	static int shift = 7;
	int bit;

	if (signal == SIGUSR1)
		bit = 0;
	else if (signal == SIGUSR2)
		bit = 1;
	else
		return;
	g_data |= (bit << shift);
	shift--;
	if (shift < 0)
	{
		if (g_data == '\0') // buraya ekleme yapılacak message yazmıyor not: newlinedan sonra karakter var ise yazma.
			write(1, "\nMessage received.", 19);
		write(1, &g_data, 1);
		g_data = 0;
		shift = 7;
	}
}

int main(void)
{
	struct sigaction var;

	write(1, "Server running on PID: ", 22);
	ft_putnbr(getpid());
	write(1, "\n", 1);

	sigemptyset(&var.sa_mask);		  // sinyal maskesini temizle
	sigaddset(&var.sa_mask, SIGUSR1); // SIGUSR1'i maskeye ekle
	sigaddset(&var.sa_mask, SIGUSR2); // SIGUSRW'yi maskeye ekle
	var.sa_flags = SA_SIGINFO;		  // 3 parametrel'i handler kullan
	var.sa_sigaction = take_signal;	  // handler fonksiyonunu ata
	sigaction(SIGUSR1, &var, NULL);
	sigaction(SIGUSR2, &var, NULL);
	write(1, "\n", 1);
	while (1)
		pause();
}
