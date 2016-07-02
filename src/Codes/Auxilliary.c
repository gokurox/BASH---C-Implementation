#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../Headers/CMD_t.h"

#include "../Headers/CD.h"
#include "../Headers/Clear.h"
#include "../Headers/Help.h"
#include "../Headers/History.h"
#include "../Headers/PID.h"
#include "../Headers/PWD.h"
#include "../Headers/Whoami.h"
#include "../Headers/Kill.h"
#include "../Headers/Piping.h"
#include "../Headers/Redirection.h"
#include "../Headers/Ls.h"

/*********************************************************************************************************************************/

void showPrompt ()
{
	char *uName = returnWhoami ();
	char *pwd = returnPWD ();

	printf ("GSHELL: %s @ %s ## ", uName, pwd);
	return;
}

void cleanInputBuffer ()
{
	char ch;
	while ((ch = getchar ()) != '\n' && ch != EOF);
	return;
}

char *inputReader ()
{
	char inputString [1024];
	fgets (inputString, 1024, stdin);

	return (strdup (inputString));
}

CMD_t *parseInput (char *inputString)
{
	int args;
	int TMP_maxArgLength;
	int TMP_strlen = strlen (inputString);
	char *TMP_string;
	CMD_t *inputCommand = NULL;

	if (inputString [TMP_strlen -1] == '\n')
		inputString [TMP_strlen -1] = '\0';
	TMP_strlen = strlen (inputString);

	int i, TMP_skp_spc, TMP_cnt_spc;
	i = 0;
	while (inputString[i] == ' ')
	{
		i ++;
	}

	TMP_skp_spc = i;
	TMP_cnt_spc = 0;
	i = TMP_strlen -1;
	
	while (inputString[i] == ' ')
	{
		i --;
	}

	for (i; i >= 0; i--)
	{
		if (inputString[i] == ' ')
			TMP_cnt_spc ++;
	}

	args = TMP_cnt_spc - TMP_skp_spc +1;

	inputCommand = (CMD_t *) malloc (sizeof (CMD_t));
	inputCommand -> argv = (char **) malloc (sizeof (char *) * args);
	inputCommand -> args = args;
	
	TMP_string = strtok (inputString, " ,");
	i = 0;
	while (TMP_string != NULL)
	{
		inputCommand -> argv[i] = (char *) malloc (sizeof (char) * strlen (TMP_string));
		strcpy (inputCommand -> argv[i], TMP_string);
		i ++;
		TMP_string = strtok (NULL, " ,");
	}

	if (i == 0)
	{
		inputCommand -> argv[0] = (char *) malloc (sizeof (char) * 5);
		strcpy (inputCommand -> argv[0], "?//?");
	}

	return inputCommand;
}

bool ifRedirection (CMD_t *inputCommand)
{
	int i;

	for (i = 0; i < inputCommand -> args; i++)
	{
		if (!strcmp (inputCommand -> argv[i], "<") || !strcmp (inputCommand -> argv[i], ">"))
		{
			return true;
		}
	}
	return false;
}

bool ifPiping (CMD_t *inputCommand)
{
	int i;

	for (i = 0; i < inputCommand -> args; i++)
	{
		if (!strcmp (inputCommand -> argv[i], "|"))
			return true;
	}
	return false;
}

bool ifReturn (CMD_t *inputCommand)
{
	if (inputCommand -> args == 1)
		if (!strcmp (inputCommand -> argv[0], "?//?"))
		{
			return true;
		}
	return false;
}

void commandDecider (CMD_t *inputCommand)
{
	char *commandName = inputCommand -> argv[0];

	if (ifRedirection (inputCommand))
	{
		// Send to Redirection
		executeRedirection (inputCommand);
		return;
	}

	if (ifPiping (inputCommand))
	{
		// Send to Piping
		executePiping (inputCommand);
		return;
	}

	if (ifReturn (inputCommand))
	{
		// Don't do anything
		return;
	}

	if (!strcmp (commandName, "help"))
	{
		// Send to help
		executeHelp (inputCommand);
		return;
	}

	if (!strcmp (commandName, "clear"))
	{
		// Send to clear
		executeClear (inputCommand);
		return;
	}

	if (!strcmp (commandName, "pwd"))
	{
		// Send to pwd
		showPWD ();
		return;
	}

	if (!strcmp (commandName, "ls"))
	{
		// Send to ls
		executeLS (inputCommand);
		return;
	}

	if (!strcmp (commandName, "whoami"))
	{
		// Send to whoami
		showWhoami ();
		return;
	}

	if (!strcmp (commandName, "cd"))
	{
		// Send to cd
		executeCD (inputCommand);
		return;
	}

	if (!strcmp (commandName, "pid"))
	{
		// Send to pid
		executePID (inputCommand);
		return;
	}

	if (!strcmp (commandName, "history"))
	{
		// Send to history
		executeHistory (inputCommand);
		return;
	}

	if (!strcmp (commandName, "kill"))
	{
		// Send to kill
		executeKill (inputCommand);
		return;
	}

	if (!strcmp (commandName, "exit"))
	{
		clearHistory ();
		exit (0);
	}

	printf ("ERROR: INVALID_COMMAND :: \"%s\" is not a recognised command.. Type \"help\" for more information\n", commandName);
	return;
}