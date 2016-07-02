#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../Headers/CMD_t.h"
#include "../Headers/Auxilliary.h"

bool validRedirection (CMD_t *);
void executeRedirection (CMD_t *);
CMD_t *givePartCommand (CMD_t *, int , int );
void continueRedirection (CMD_t *, int , int , int );
void inputRedirection (CMD_t *, CMD_t *);
void outputRedirection (CMD_t *, CMD_t *);
void streamCopy (FILE *, FILE *);

bool validRedirection (CMD_t *inputCommand)
{
	bool current, prev;
	int i;

	current = false;
	prev = false;

	if (!strncmp (inputCommand -> argv[0], "<", 1) || !strncmp (inputCommand -> argv[0], ">", 1))
		return false;
	
	prev = false;
	for (i = 1; i < inputCommand -> args -1; i++)
	{
		if (!strncmp (inputCommand -> argv[i], "<", 1) || !strncmp (inputCommand -> argv[i], ">", 1))
			current = true;
		else
			current = false;

		if (current == true && prev == true)
			return false;

		prev = current;
	}

	if (!strncmp (inputCommand -> argv[i], "<", 1) || !strncmp (inputCommand -> argv[i], ">", 1))
		return false;

	return true;
}

void executeRedirection (CMD_t *inputCommand)
{
	int redirectionCount, inputRedirectionCount, outputRedirectionCount;
	int i;

	redirectionCount = 0;
	inputRedirectionCount = 0;
	outputRedirectionCount = 0;

	for (i = 0; i < inputCommand -> args; i++)
	{
		if (!strncmp (inputCommand -> argv[i], ">", 1) || !strncmp (inputCommand -> argv[i], "<", 1))
		{
			redirectionCount ++;

			if (!strncmp (inputCommand -> argv[i], ">", 1))
				outputRedirectionCount ++;
			else
				inputRedirectionCount ++;
		}
	}

	if (validRedirection (inputCommand))
	{
		continueRedirection (inputCommand, redirectionCount, inputRedirectionCount, outputRedirectionCount);
		return;
	}
	else
	{
		printf ("ERROR: INVALID_REDIRECTION\n");
		return;
	}

	return;
}

CMD_t *givePartCommand (CMD_t *inputCommand, int start, int end)
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

void continueRedirection (CMD_t *inputCommand, int rC, int irC, int orC)
{
	bool TMP_flag = false;
	int cmd1_start, cmd1_end;
	int cmd2_start, cmd2_end;
	int args = inputCommand -> args;
	char **argv = inputCommand -> argv;
	int i, j;
	int redirectionType;

	CMD_t *leftCommand;
	CMD_t *rightFile;

	i = 0;

	cmd1_start = i;

	for (i = 0; i < args; i++)
	{
		if (!strncmp (argv[i], ">", 1) || !strncmp (argv[i], "<", 1))
			break;
	}

	cmd1_end = i -1;
	cmd2_start = i +1;
	cmd2_end = i +1;		// should be just a filename


	leftCommand = givePartCommand (inputCommand, cmd1_start, cmd1_end);
	rightFile = givePartCommand (inputCommand, cmd2_start, cmd2_end);

	rC --;

	if (!strncmp (argv[i], "<", 1))
	{
		inputRedirection (leftCommand, rightFile);
	}
	else
	{
		outputRedirection (leftCommand, rightFile);
	}

	return;
}

void inputRedirection (CMD_t *leftCommand, CMD_t *rightFile)
{
	int fd[2];
	
	if (pipe (fd) == -1)
		printf ("ERROR: PIPE_DUMP :: can't create pipes for \"IN_REDIRECTION\"\n");

	FILE *inputStream = fopen (rightFile -> argv[0], "r");
	FILE *fdOStream = fdopen (fd[1], "w");

	streamCopy (inputStream, fdOStream);

	int forkValue = fork ();

	if (forkValue < 0)
	{
		// *** Can't create a new process ***
		printf ("FORK EXIT_STATUS %d\n", forkValue);
	}
	else if (forkValue > 0)
	{
		wait (NULL);
		return;
	}
	else if (forkValue == 0)
	{
		close (0);
		dup (fd[0]);

		commandDecider (leftCommand);

		exit (0);
	}
}

void outputRedirection (CMD_t *leftCommand, CMD_t *rightFile)
{
	int fd[2];
	
	if (pipe (fd) == -1)
		printf ("ERROR: PIPE_DUMP :: can't create pipes for \"IN_REDIRECTION\"\n");

	FILE *outputStream = fopen (rightFile -> argv[0], "w+");
	FILE *fdIStream = fdopen (fd[0], "r");

	int forkValue = fork ();

	if (forkValue < 0)
	{
		// *** Can't create a new process ***
		printf ("FORK EXIT_STATUS %d\n", forkValue);
	}
	else if (forkValue > 0)
	{
		wait (NULL);

		streamCopy (fdIStream, outputStream);
	}
	else if (forkValue == 0)
	{
		close (1);
		dup (fd[1]);

		commandDecider (leftCommand);
		printf ("%c", EOF);

		exit (0);
	}

	fclose (outputStream);
	fclose (fdIStream);
}

void streamCopy (FILE *fromStream, FILE *toStream)
{
	if (fromStream == NULL)
	{
		printf ("ERROR: REDIRECTION: INVALID_FROM_STREAM\n");
		return;
	}

	if (toStream == NULL)
	{
		printf ("ERROR: REDIRECTION: INVALID_TO_STREAM\n");
		return;
	}

	fseek (fromStream, 0, SEEK_SET);
	fseek (toStream, 0, SEEK_SET);

	char ch;

	while ((ch = fgetc (fromStream)) != EOF && !feof (fromStream))
	{
		fprintf (toStream, "%c", ch);
	}

	return;
}