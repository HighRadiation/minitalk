/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 19:28:46 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/16 21:22:15 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char g_data;

static void	take_signal(int signal)
{
	int	bit;
	static int shift;

	shift = 7;
	if (signal == SIGUSR1)
		bit = 0;
	if (signal == SIGUSR2)
		bit = 1;
	else
		return ;
	g_data |= (bit << shift); // bit'i current char'a ekle.
	shift--;
	if (shift < 0)
	{
		write(1, &g_data, 1); // 8 bit tamamlandı bir karakter yaz.
		g_data = 0; // sıfırla baştan
		shift = 7;
	}
}

int take_process_id(int pid)
{

}

int main()
{
	write(1, "Server runnig on PID: ", 22);
	ft_putnbr(take_process_id());
	write(1, "\n", 1);
}
