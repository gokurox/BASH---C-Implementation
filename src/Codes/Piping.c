#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../Headers/CMD_t.h"
#include "../Headers/Auxilliary.h"

bool validPiping (CMD_t *);
void executeRedirection (CMD_t *);
CMD_t *givePartCommand_pipe (CMD_t *, int , int );
void continuePiping (CMD_t *, int );
void pipeIt (CMD_t *, CMD_t *);

bool validPiping (CMD_t *inputCommand)
{
	bool current, prev;
	int i;

	current = false;
	prev = false;

	if (!strncmp (inputCommand -> argv[0], "|", 1))
		return false;
	
	prev = false;
	for (i = 1; i < inputCommand -> args -1; i++)
	{
		if (!strncmp (inputCommand -> argv[i], "|", 1))
			current = true;
		else
			current = false;

		if (current == true && prev == true)
			return false;

		prev = current;
	}

	if (!strncmp (inputCommand -> argv[i], "|", 1))
		return false;


	return true;
}

void executePiping (CMD_t *inputCommand)
{
	int pipingCount;
	int i;

	pipingCount = 0;

	for (i = 0; i < inputCommand -> args; i++)
	{
		if (!strncmp (inputCommand -> argv[i], "|", 1))
		{
			pipingCount ++;
		}
	}


	if (validPiping (inputCommand))
	{
		continuePiping (inputCommand, pipingCount);
		return;
	}
	else
	{
		printf ("ERROR: INVALID_PIPING\n");
		return;
	}

	return;
}

CMD_t *givePartCommand_pipe (CMD_t *inputCommand, int start, int end)
{
	CMD_t *partCommand = (CMD_t *) malloc (sizeof (CMD_t));

	partCommand -> args = (end - start +1);
	partCommand -> argv = (char **) malloc (sizeof (char *) * partCommand -> args);

	int i, j;
	for (i = start, j = 0; i <= end; i++, j++)
	{
		partCommand -> argv[j] = (char *) malloc (sizeof (char) * strlen (inputCommand -> argv[i]));
		strcpy (partCommand -> argv[j], inputCommand -> argv[i]);
	}

	return partCommand;
}

void continuePiping (CMD_t *inputCommand, int pC)
{
	int cmd1_start, cmd1_end;
	int cmd2_start, cmd2_end;
	int args = inputCommand -> args;
	char **argv = inputCommand -> argv;
	int i;

	CMD_t *leftCommand;
	CMD_t *rightCommand;

	i = 0;

	cmd1_start = i;

	for (i = 0; i < args; i++)
	{
		if (!strncmp (argv[i], "|", 1))
			break;
	}

	cmd1_end = i -1;
	cmd2_start = i +1;
	for (i = i +1; i < args; i++)
	{
		if (!strncmp (argv[i], "|", 1))
			break;
		if (i == args -1)
			break;
	}

	cmd2_end = i;


	leftCommand = givePartCommand_pipe (inputCommand, cmd1_start, cmd1_end);
	rightCommand = givePartCommand_pipe (inputCommand, cmd2_start, cmd2_end);


	pC --;

	pipeIt (leftCommand, rightCommand);

	return;
}

void pipeIt (CMD_t *leftCommand, CMD_t *rightCommand)
{
	int fd[2];
	
	if (pipe (fd) == -1)
		printf ("ERROR: PIPE_DUMP :: can't create pipes for \"PIPING\"\n");

	FILE *fdIStream = fdopen (fd[0], "r");
	FILE *fdOStream = fdopen (fd[1], "w");

	int forkValue = fork ();

	if (forkValue < 0)
	{
		// *** Can't create a new process ***
		printf ("FORK EXIT_STATUS %d\n", forkValue);
	}
	else if (forkValue > 0)
	{
		wait (NULL);
	}
	else if (forkValue == 0)
	{
		close (1);
		dup (fd[1]);

		commandDecider (leftCommand);


		exit (0);
	}

	forkValue = fork ();

	if (forkValue < 0)
	{
		// *** Can't create a new process ***
		printf ("FORK EXIT_STATUS %d\n", forkValue);
	}
	else if (forkValue > 0)
	{
		wait (NULL);
	}
	else if (forkValue == 0)
	{
		close (0);
		dup (fd[0]);
		commandDecider (leftCommand);
		exit (0);
	}

	return;
}