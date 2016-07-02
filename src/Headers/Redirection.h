#ifndef REDIRECTION_H
#define REDIRECTION_H

bool validRedirection (CMD_t *);
void executeRedirection (CMD_t *);
CMD_t *givePartCommand (CMD_t *, int , int );
void continueRedirection (CMD_t *, int , int , int );
void inputRedirection (CMD_t *, CMD_t *);
void outputRedirection (CMD_t *, CMD_t *);
void streamCopy (FILE *, FILE *);

#endif