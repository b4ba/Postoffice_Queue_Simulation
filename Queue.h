#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statistics.h"
/* Initialising structs */
struct QueueNode
{
        int toleranceLimit;
	int servingTime;
	int timeSpentWaiting;
        struct QueueNode *nextNode;
	struct QueueNode *previousNode;
};

typedef struct QueueNode Customer;

struct Queue
{
        int Count;
        Customer *Head;
        Customer *Tail;
};

typedef struct Queue QUEUE;

/* Declaring function prototypes */
extern QUEUE* InitialiseQueue();
extern int isEmpty(QUEUE *);
extern void enqueue(QUEUE *, Customer *);
extern Customer* dequeue(QUEUE *);
extern void printQueue(QUEUE *);
extern void freeQueue(Customer *);
extern Customer* nextNode(Customer* );
extern int removeQueueNode(QUEUE *, Customer *);
extern int removeFromQueue(QUEUE *);
#endif
