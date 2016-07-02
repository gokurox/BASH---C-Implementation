#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

#include "../Headers/CMD_t.h"

#include "../Headers/Auxilliary.h"
#include "../Headers/History.h"
#include "../Headers/Newterm.h"

/*********************************************************************************************************************************/

void sigproc ();
void initiateShell (void );
void shellLoop (void );
void commandDecider (CMD_t *);

/*********************************************************************************************************************************/

int main (void )
{
	signal (SIGINT, SIG_IGN);
	// executeNewterm ();
	initiateShell ();

	return 0;
}

void sigproc ()
{
	printf ("\n\t\t\tctrl-C Handled Elegantly :D\n");
	shellLoop ();
}

void initiateShell (void )
{
	clearConsole ();

	printf ("*****************************************************************************************************\n");
	printf ("\n  *\t\t\t\t\t\tGSHELL\t\t\t\t\t\t  *\n");
	printf ("  *\t\t\t\tImplemented By:: Gursimran Singh __ 2014041\t\t\t  *\n\n");
	printf ("*****************************************************************************************************\n");
	
	printf ("\n");

	shellLoop ();

	return;
}

void shellLoop (void )
{
	char *inputString;
	CMD_t *inputCommand;

	while (1)
	{
		showPrompt ();
		inputString = inputReader ();
		inputCommand = parseInput (inputString);
		commandDecider (inputCommand);
		addToHistory (inputCommand);
	}

	return;
}