#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../Headers/CMD_t.h"

#define CMD_CLEAR "/usr/bin/clear"

/*********************************************************************************************************************************/

void executeClear (CMD_t *);
void clearConsole (void );

/*********************************************************************************************************************************/

void executeClear (CMD_t *inputCommand)
{
	if (inputCommand -> args == 1)
		clearConsole ();
	else
		printf ("ERROR: ARGS_MISMATCH :: \"clear\" does not take any arguments.. Type \"help\" for more information\n");
}

void clearConsole (void )
{
	int forkValue = fork ();				// Create a new process

	if (forkValue < 0)					// Error
	{
		/* *** Can't create a new process *** */
		printf ("FORK EXIT_STATUS %d :: Cant execute \"clear\"\n", forkValue);
	}
	else if (forkValue > 0)					// Parent
	{
		wait (NULL);					// Wait for CMD_CLEAR to execute
		return;						// End the clearConsole function
	}
	else if (forkValue == 0)				// Child
	{
		// *** Execute clear command ***
		
		char *clearArgs[2];
		clearArgs[0] = strdup (CMD_CLEAR);
		clearArgs[1] = NULL;				// End of Arguments
		execvp (clearArgs[0], clearArgs);		// Execute clear command
		exit (-1);					// End child if exec fails
	}
}