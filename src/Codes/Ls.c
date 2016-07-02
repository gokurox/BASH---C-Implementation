#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../Headers/CMD_t.h"

#define CMD_LS "/bin/ls"


void executeLS (CMD_t *inputCommand)
{
	int args = inputCommand -> args;
	char *argv[args +1];

	int i;
	argv[0] = strdup (CMD_LS);
	for (i = 1; i < args; i++)
	{
		argv[i] = strdup (inputCommand -> argv[i]);
	}
	argv[args] = NULL;

	int forkValue = fork ();

	if (forkValue < 0)
	{
		printf ("FORK EXIT_STATUS %d :: Cant execute \"ls\"\n", forkValue);
	}
	else if (forkValue > 0)
	{
		wait (NULL);
	}
	else if (forkValue == 0)
	{
		execvp (argv[0], argv);
		exit (-1);
	}
}