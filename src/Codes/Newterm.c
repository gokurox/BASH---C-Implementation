#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define CMD_GNOME_TERMINAL "/usr/bin/gnome-terminal"

/*********************************************************************************************************************************/

void executeNewterm ()
{
	int forkValue = fork ();				// Create a new process

	if (forkValue < 0)					// Error
	{
		/* *** Can't create a new process *** */
		printf ("FORK EXIT_STATUS %d :: Cant execute \"gnome-terminal\"\n", forkValue);
	}
	else if (forkValue > 0)					// Parent
	{
		wait (NULL);					// Wait for CMD_GNOME_TERMINAL to execute
		return;
	}
	else if (forkValue == 0)				// Child
	{
		// *** Execute gnome-terminal command ***
		
		char *newTermArgs[2];
		
		newTermArgs[0] = strdup (CMD_GNOME_TERMINAL);
		newTermArgs[1] = NULL;

		execvp (newTermArgs[0], newTermArgs);
		exit (-1);					// End child if exec fails
	}
}