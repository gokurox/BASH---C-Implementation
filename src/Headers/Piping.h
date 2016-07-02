#ifndef PIPING_H
#define PIPING_H

bool validPiping (CMD_t *);
void executeRedirection (CMD_t *);
CMD_t *givePartCommand_pipe (CMD_t *, int , int );
void continuePiping (CMD_t *, int );
void pipeIt (CMD_t *, CMD_t *);

#endif