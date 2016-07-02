#include <stdio.h>
#include <unistd.h>

#include "../Headers/CMD_t.h"

/*********************************************************************************************************************************/

void executePID (CMD_t *);
int returnPID (void );
void showPID (void );

/*********************************************************************************************************************************/

void executePID (CMD_t *inputCommand)
{
	if (inputCommand -> args == 1)
		showPID ();
	else
		printf ("ERROR: ARGS_MISMATCH :: \"pid\" does not take any arguments.. Type \"help\" for more information\n");
}

int returnPID (void )
{
	int pid = getpid ();
	return pid;
}

void showPID (void )
{
	int pid = returnPID ();
	printf ("PID:\t%d\n", pid);
	return;
}