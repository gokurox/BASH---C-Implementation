#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../Headers/CMD_t.h"
#include "../Headers/History.h"
#include "../Headers/Auxilliary.h"

HLL *HLL_START = NULL;
HLL *HLL_END = NULL;
HLL *RAND_SEEK = NULL;

HLL *freeMem (HLL *TMP)
{
	int CMD_args = TMP -> Command -> args;
	char **CMD_argv = TMP -> Command -> argv;

	int i;
	for (i = 0; i < CMD_args; i++)
	{
		free (CMD_argv[i]);
		CMD_argv[i] = NULL;
	}

	free (CMD_argv);		CMD_argv = NULL;
	free (TMP -> Command);		TMP -> Command = NULL;
	
	TMP -> prev = NULL;
	TMP -> next = NULL;

	return NULL;
}

char *joinCommand (CMD_t *inputCommand)
{
	char completeCommand [1024] = "";
	int args = inputCommand -> args;
	
	int i;
	for (i = 0; i < args; i++)
	{
		strcat (completeCommand, inputCommand -> argv[i]);
		if (i != args -1)
			strcat (completeCommand, " ");
	}

	return (strdup (completeCommand));
}

void insertAtBeginning (CMD_t *inputCommand)
{
	if (HLL_START == NULL && HLL_END == NULL)
	{
		HLL_START = (HLL *) malloc (sizeof (HLL));
		HLL_START -> Command = inputCommand;
		HLL_START -> prev = NULL;
		HLL_START -> next = NULL;
		HLL_END = HLL_START;
		return;
	}
	else
	{
		HLL *TMP = (HLL *) malloc (sizeof (HLL));
		TMP -> Command = inputCommand;
		TMP -> prev = NULL;
		
		TMP -> next = HLL_START;
		HLL_START -> prev = TMP;

		HLL_START = TMP;
		TMP = NULL;
		return;
	}

	printf ("ERROR: INSERT_AT_BEGINNING\n");
	return;
}

void insertAtEnd (CMD_t *inputCommand)
{
	if (HLL_END == NULL && HLL_START == NULL)
	{
		HLL_END = (HLL *) malloc (sizeof (HLL));
		HLL_END -> Command = inputCommand;
		HLL_END -> prev = NULL;
		HLL_END -> next = NULL;
		HLL_START = HLL_END;
		return;
	}
	else
	{
		HLL *TMP = (HLL *) malloc (sizeof (HLL));
		TMP -> Command = inputCommand;
		
		TMP -> prev = HLL_END;
		HLL_END -> next = TMP;
		
		TMP -> next = NULL;
		
		HLL_END = TMP;
		TMP = NULL;
		return;
	}

	printf ("ERROR: INSERT_AT_END\n");
	return;
}

void deleteFromBeginning (void )
{
	if (HLL_START == NULL && HLL_END == NULL)
	{
		return;
	}
	else if (HLL_START == HLL_END)
	{
		HLL *TMP = HLL_START;
		HLL_START = NULL;
		HLL_END = NULL;
		
		TMP = freeMem (TMP);

		return;
	}
	else
	{
		HLL *TMP = HLL_START;
		HLL_START = HLL_START -> next;
		HLL_START -> prev = NULL;

		TMP = freeMem (TMP);

		return;
	}

	printf ("ERROR: DELETE_FROM_BEGINNING\n");
	return;
}

void deleteFromEnd (void )
{
	if (HLL_END == NULL && HLL_START == NULL)
	{
		return;
	}
	else if (HLL_END == HLL_START)
	{
		HLL *TMP = HLL_END;
		HLL_END = NULL;
		HLL_START = NULL;

		TMP = freeMem (TMP);

		return;
	}
	else
	{
		HLL *TMP = HLL_END;
		HLL_END = HLL_END -> prev;
		HLL_END -> next = NULL;

		TMP = freeMem (TMP);

		return;
	}

	printf ("ERROR: DELETE_FROM_END\n");
	return;
}

void printHistory (void )
{
	HLL *Iterator = HLL_START;
	int indexCount = 1;

	while (Iterator != NULL)
	{
		printf ("\t%d\t\t%s\n", indexCount, joinCommand (Iterator -> Command));
		indexCount ++;
		Iterator = Iterator -> next;
	}

	return;
}

void printCommandAt (int printIndex)
{
	HLL *Iterator = HLL_START;
	int indexCount = 1;

	if (printIndex <= 0)
		printIndex = 1;

	while (indexCount < printIndex && Iterator != NULL)
	{
		indexCount ++;
		Iterator = Iterator -> next;
	}

	if (Iterator == NULL)
		printf ("No entry at index %d\n", printIndex);
	else
		printf ("\t%d\t\t%s\n", indexCount, joinCommand (Iterator -> Command));

	return;
}

void printCommandAtRandSeek (void )
{
	HLL *Iterator = RAND_SEEK;
	if (Iterator != NULL)
		printf ("\t\t%s\n", joinCommand (Iterator -> Command));

}

void clearHistory (void )
{
	while (HLL_START != NULL)
	{
		deleteFromBeginning ();
	}

	HLL_START = NULL;
	HLL_END = NULL;
	RAND_SEEK = NULL;
}

/*********************************************************************************************************************************/

void addToHistory (CMD_t *inputCommand)
{
	if (ifReturn (inputCommand))
		return;

	insertAtEnd (inputCommand);
	return;
}

void executeHistory (CMD_t *inputCommand)
{
	/*
	 * history :: print history
	 * -c :: clear history
	 * -n <number> :: print at number
	 */

	if (inputCommand -> args == 1)
	{
		printHistory ();
		return;
	}

	if (inputCommand -> args == 2)
	{
		if (!strncmp (inputCommand -> argv[1], "-c", 2))
		{
			clearHistory ();
			return;
		}

		if (!strncmp (inputCommand -> argv[1], "-n", 2))
		{
			printf ("ERROR: ARGS_MISMATCH :: One more argument required \"history -n <number>\"\n");
			return;
		}

		printf ("ERROR: INVALID_ARG :: argument \"%s\" not recognized\n", inputCommand -> argv[1]);
		return;
	}

	if (inputCommand -> args == 3)
	{
		if (!strncmp (inputCommand -> argv[1], "-n", 2))
		{
			int number = atoi (inputCommand -> argv[2]);
			printCommandAt (number);
			return;
		}
	}

	printf ("ERROR: INVALID_ARGS :: Type \"help\" for more information\n");
	return;
}