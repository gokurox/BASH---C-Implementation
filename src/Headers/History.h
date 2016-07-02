#ifndef HISTORY_H
#define HISTORY_H

typedef struct _HistoryList
{
	CMD_t *Command;

	struct _HistoryList *prev;
	struct _HistoryList *next;
} HLL;

void insertAtBeginning (CMD_t *);
void insertAtEnd (CMD_t *);
void deleteFromBegining (void );
void deleteFromEnd (void );
void printHistory (void );
void printCommandAt (int );
void printCommandAtRandSeek (void );
void addToHistory (CMD_t *);
void clearHistory (void );

#endif