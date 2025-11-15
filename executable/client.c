/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: boksuz <boksuz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:27:59 by boksuz            #+#    #+#             */
/*   Updated: 2025/11/15 19:34:56 by boksuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// mesaj gönderecek ama nasıl?
// mesajı göndermem lazım, hangi fonksiyona gidecek?

char send_to_server(char str)
{

}



// string'in içinde dolaşacak.
void procces_message(char *str)
{
	int 			i;
	static char 	letter;

	i = 0;
	while (str[i])
	{
		letter = ft_atoi(str);
		i++;
	}
	send_to_server(letter);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, '\n', 1);
		return (0);
	}
	procces_message(argv);
}
