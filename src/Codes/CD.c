#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../Headers/CMD_t.h"

/*********************************************************************************************************************************/

void executeCD (CMD_t *inputCommand)
{
	char chdirPath[1024] = "";

	if (inputCommand -> args == 2)
	{
		strcat (chdirPath, inputCommand -> argv[1]);
		chdir (chdirPath);
	}
	else if (inputCommand -> args < 2)
	{
		printf ("ERROR: ARGS_MISMATCH :: \"cd\" needs 1 argument\n");
	}
	else if (inputCommand -> args > 2)
	{
		printf ("ERROR: ARGS_MISMATCH :: \"cd\" does not take more than one argument\n");

	}
}