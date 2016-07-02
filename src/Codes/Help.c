#include <stdio.h>

#include "../Headers/CMD_t.h"

/*********************************************************************************************************************************/

void executeHelp (CMD_t *);
void showHelp (void );

/*********************************************************************************************************************************/

void executeHelp (CMD_t *inputCommand)
{
	if (inputCommand -> args == 1)
		showHelp ();
	else
		printf ("ERROR: ARGS_MISMATCH :: \"help\" does not take any arguments.. Type \"help\" for more information\n");
}

void showHelp (void )
{
	printf ("\n");
	printf ("\t\tCommands that G_Shell Supports are ::\n\n");
	
	printf ("\t\thelp\t\tTo list all the Implemented Commands\n");
	printf ("\t\t\t\"help\" takes NO Arguments\n");

	printf ("\t\tclear\t\tTo clear the console\n");
	printf ("\t\t\t\"clear\" takes NO Arguments\n");

	printf ("\t\tpwd\t\tTo show the Present Working Directory\n");
	printf ("\t\t\t\"pwd\" takes NO Arguments\n");

	printf ("\t\tls\t\tTo show the files present in PWD\n");
	printf ("\t\t\t\"ls\" takes -a -l and others as in BASH\n");

	printf ("\t\twhoami\t\tTo show the current User\n");
	printf ("\t\t\t\"whoami\" takes NO Arguments\n");

	printf ("\t\tcd\t\tTo change the Present Working Directories\n");
	printf ("\t\t\t\"cd\" takes only one Argument i.e. the Path\n");

	printf ("\t\tpid\t\tTo show the cureent process's PID\n");
	printf ("\t\t\t\"pid\" takes NO Arguments\n");

	printf ("\t\thistory\t\tTo show previously invoked commands\n");
	printf ("\t\t\t\"history\" takes -c, -n <number> to clear and show command at index number\n");

	printf ("\t\tkill\t\tTo kill process with a give PID\n");
	printf ("\t\t\t\"kill\" takes Arguments as in BASH\n");

	printf ("\t\texit\t\tTo exit the GShell\n");
	printf ("\t\t\t\"exit\" takes NO Arguments\n");

	printf ("\n\t\tRedirection and Piping are also supported\n");
	printf ("\t\t\t(Single Redirection and Single Piping Only)\n");
	printf ("\t\tReturn is handled Elegantly\n");
	printf ("\t\tCtrl-C is handled Elegantly\n");
	printf ("\t\t\tCtrl-C does not have any effect on working of GShell\n");
	printf ("\t\tErronous Commands are handled well\n");
	printf ("\n");
	return;
}