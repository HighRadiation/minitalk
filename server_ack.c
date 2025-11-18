 4+#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

static volatile sig_atomic_t g_char = 0;
static volatile sig_atomic_t g_bit = 0;

void handler(int sig, siginfo_t *info, void *ucontext)
{
	(void)ucontext;
	pid_t client_pid = info->si_pid;

	/* Interpret SIGUSR1 as bit 0, SIGUSR2 as bit 1 */
	if (sig == SIGUSR2)
		g_char |= (1 << g_bit);

	g_bit++;

	if (g_bit == 8)
	{
		char c = (char)g_char;
		if (c == '\0')
		{
			const char *msg = "\nMessage received.\n";
			/* Correct length: 19 bytes */
			write(1, msg, 19);
		}
		else
		{
			write(1, &c, 1);
		}
		g_char = 0;
		g_bit = 0;
	}

	/* Send ACK for this bit back to the client */
	if (client_pid > 0)
		kill(client_pid, SIGUSR1);
}

int main(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("sigaction SIGUSR1");
		return EXIT_FAILURE;
	}
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		perror("sigaction SIGUSR2");
		return EXIT_FAILURE;
	}

	printf("Server (ACK) PID: %d\n", getpid());
	fflush(stdout);

	while (1)
		pause();

	return 0;
}
