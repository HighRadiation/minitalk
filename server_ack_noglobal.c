#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

/*
 * No file-scope globals. Use sigwaitinfo() in main and keep per-client state
 * in a linked list local to main. Each received signal includes the sender PID
 * so we can track multiple clients without globals.
 */

struct client_state
{
	pid_t pid;
	unsigned char ch;
	int bit;
	struct client_state *next;
};

int main(void)
{
	sigset_t set;
	if (sigemptyset(&set) == -1)
	{
		perror("sigemptyset");
		return EXIT_FAILURE;
	}
	if (sigaddset(&set, SIGUSR1) == -1)
	{
		perror("sigaddset SIGUSR1");
		return EXIT_FAILURE;
	}
	if (sigaddset(&set, SIGUSR2) == -1)
	{
		perror("sigaddset SIGUSR2");
		return EXIT_FAILURE;
	}

	/* Block these signals so they are delivered to sigwaitinfo() */
	if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
	{
		perror("sigprocmask");
		return EXIT_FAILURE;
	}

	printf("Server (ACK no-global) PID: %d\n", getpid());
	fflush(stdout);

	struct client_state *head = NULL;

	for (;;)
	{
		siginfo_t info;
		int sig = sigwaitinfo(&set, &info);
		if (sig == -1)
		{
			/* Interrupted or error; continue waiting */
			continue;
		}

		pid_t client_pid = info.si_pid;
		if (client_pid <= 0)
			continue;

		/* Find or create client state in local list */
		struct client_state *cur = head;
		while (cur && cur->pid != client_pid)
			cur = cur->next;
		if (!cur)
		{
			cur = malloc(sizeof(*cur));
			if (!cur)
				continue; /* OOM: drop signal */
			cur->pid = client_pid;
			cur->ch = 0;
			cur->bit = 0;
			cur->next = head;
			head = cur;
		}

		/* Interpret SIGUSR1 as bit 0, SIGUSR2 as bit 1 */
		if (sig == SIGUSR2)
			cur->ch |= (1 << cur->bit);

		cur->bit++;

		if (cur->bit == 8)
		{
			char c = (char)cur->ch;
			if (c == '\0')
			{
				const char *msg = "\nMessage received.\n";
				write(1, msg, 19);
			}
			else
			{
				write(1, &c, 1);
			}
			cur->ch = 0;
			cur->bit = 0;
		}

		/* ACK this bit back to the client */
		kill(client_pid, SIGUSR1);
	}

	/* Not reached, but tidy: free list if needed */
	/* while (head) { struct client_state *n = head->next; free(head); head = n; } */

	return 0;
}
