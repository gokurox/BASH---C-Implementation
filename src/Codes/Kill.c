#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../Headers/CMD_t.h"

#define CMD_KILL "/bin/kill"

/*********************************************************************************************************************************/

void executeKill (CMD_t *inputCommand)
{
	int forkValue = fork ();				// Create a new process

	if (forkValue < 0)					// Error
	{
		/* *** Can't create a new process *** */
		printf ("FORK EXIT_STATUS %d :: Cant execute \"kill\"\n", forkValue);
	}
	else if (forkValue > 0)					// Parent
	{
		wait (NULL);					// Wait for CMD_KILL to execute
		return;
	}
	else if (forkValue == 0)				// Child
	{
		// *** Execute kill command ***
		
		char *killArgs[inputCommand -> args +1];
		
		int i;

		killArgs[0] = strdup (CMD_KILL);
		for (i = 1; i < inputCommand -> args; i++)
		{
			killArgs[i] = strdup (inputCommand -> argv[i]);
		}
		killArgs[i] = NULL;


		execvp (killArgs[0], killArgs);
		exit (-1);					// End child if exec fails
	}
}