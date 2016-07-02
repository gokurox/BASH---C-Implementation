#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "../Headers/CMD_t.h"

#define CMD_WHOAMI "/usr/bin/whoami"

/*********************************************************************************************************************************/

char *returnWhoami (void );
void showWhoami (void );

/*********************************************************************************************************************************/

char *returnWhoami (void )
{
	int fd[2];
	char whoami[1024];
	
	if (pipe (fd) == -1)
		printf ("ERROR: PIPE_DUMP :: can't create pipes for \"whoami\"\n");

	int forkValue = fork ();

	if (forkValue < 0)						// Error
	{
		// *** Can't create a new process ***
		printf ("FORK EXIT_STATUS %d :: cant execute \"clear\"\n", forkValue);
	}
	else if (forkValue > 0)						// Parent
	{
		wait (NULL);
		
		FILE *stream = fdopen (fd[0], "r");
		fscanf (stream, "%s", whoami);
		
		return (strdup (whoami));				// End the returnPWD function
	}
	else if (forkValue == 0)					// Child
	{
		close (1);
		dup (fd[1]);

		// *** Execute pwd command ***
		char *pwdArgs;
		pwdArgs = strdup (CMD_WHOAMI);
		execlp (pwdArgs, pwdArgs, NULL);			// Execute clear command
		exit (-1);						// End child if exec fails
	}
}

void showWhoami (void )
{
	char *whoami = returnWhoami ();
	printf ("%s\n", whoami);
	return;
}