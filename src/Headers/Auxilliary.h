#ifndef AUXILLIARY_H
#define AUXILLIARY_H

void showPrompt ();
void cleanInputBuffer ();
char *inputReader ();
CMD_t *parseInput (char *);
bool ifRedirection (CMD_t *);
bool ifPiping (CMD_t *);
bool ifReturn (CMD_t *);
void commandDecider (CMD_t *);

#endif