#include "Queue.h"

/* Create a new empty Queue data structure */
extern QUEUE *InitialiseQueue()
{
	QUEUE *newQueue = NULL;
	
	newQueue = (QUEUE *)malloc(sizeof(QUEUE)); /* Dynamically allocate memory to queue */
	if(newQueue == NULL)
	{
		fprintf(stderr, "Cannot successfully allocate memory to new Queue\n");
		exit(-1); 
	}
	/* Initialise queue attributes */
	newQueue->Count = 0;
	newQueue->Head = NULL;
	newQueue->Tail = NULL;
	return newQueue;

}


/* Returns true (0) if queue is empty */
extern int isEmpty(QUEUE *Q)
{	
	return (Q->Count == 0);
}

/* Function for adding a node to the queue */
extern void enqueue(QUEUE *Q, Customer *C)
{
        if(Q->Head == NULL){
                Q->Head = Q->Tail = C;
		C->nextNode = NULL;
		C->previousNode = NULL;
		
        }else{

		C->previousNode = Q->Tail;
                (Q->Tail)->nextNode = C;
		Q->Tail = C;
        }
        Q->Count++;
}

/* Removing a node from the queue */
extern Customer* dequeue(QUEUE *Q)
{

        if(!isEmpty(Q))
        {
                Customer* temp = Q->Head;
                Q->Head = (Q->Head)->nextNode;
                if(Q->Head)
		{
			Q->Head->previousNode = NULL;
		}		
		Q->Count--;
		
                if(!Q->Head)
                {
			free(Q->Head);
			Q->Tail = NULL;
		}
		
		return temp;
        }
	return NULL;
}

/* Function used to remove specific node from Queue */
extern int removeQueueNode(QUEUE *Q, Customer *target)
{
	if(isEmpty(Q))
	{
		return EXIT_SUCCESS;
	}
	if(!target)
	{
		return EXIT_FAILURE;
	}
	if(Q->Head == Q->Tail)
	{
		free(dequeue(Q));
		return EXIT_SUCCESS;
	}
	if(!target->previousNode)
	{
		free(dequeue(Q));
		return EXIT_SUCCESS;
	}
	if(!target->nextNode)
	{
		if(!target->previousNode)
		{
			fprintf(stderr, "Error: Queue is not working correctly\n");
			return EXIT_FAILURE;
		}

		Customer *temp = Q->Tail;
		Q->Tail->previousNode->nextNode = NULL;
		Q->Tail = temp->previousNode;
		free(temp);
		Q->Count--;
		return EXIT_SUCCESS;
	}
	if(Q->Count > 2)
	{
		/* Reassigning pointers to nodes after target node is removed */
		target->previousNode->nextNode = target->nextNode;
		target->nextNode->previousNode = target->previousNode;
		free(target);
		Q->Count--;
		return EXIT_SUCCESS;

	}
	
	return EXIT_FAILURE;

}

/* Function for removing a customer from the middle of the queue */
extern int removeFromQueue(QUEUE *Q)
{
	
	int customersLeft = 0;
	Customer *target = Q->Head;
/*	int i;
	for(i = 0; i < Q->Count; i++) */
	while(target!=NULL) 
	{
		target->toleranceLimit--;
		if(target->toleranceLimit <= 0)
		{
			/* Removing target node from queue and reassigning it to head */
			removeQueueNode(Q, target);
			/*target = Q->Head;*/
			customersLeft++;
		}
		target = target->nextNode;
	}	
	return customersLeft;
}

/* Function fro printing out the queue */
extern void printQueue(QUEUE* Q)
{
    if (isEmpty(Q)) {
        printf("THE QUEUE IS EMPTY!\n");
        return;
    }
    Customer* current = Q->Head;
    while (current != NULL) 	
    {
	
	if(current == Q->Head)
	{
		printf("HEAD-->");
	}
	if(current == Q->Tail)
	{
		printf("TAIL-->");
	}	
		printf("TL: %d, ST: %d,Memory Location Previous: %p,  Memory Location Current: %p, Memory Location Next: %p\n", (current->toleranceLimit), (current->servingTime), (current->previousNode), (current), (current->nextNode));
        
	if(current==Q->Tail)
	{
		break;
	}
	
	current = current->nextNode;
    }


    printf("\n");
}

/* Function for freeing the nodes insie the queue */
extern void freeQueue( Customer* C)
{
	if(C->nextNode != NULL) 
	{
		 freeQueue( nextNode(C));
	}
	free(C);
}
/* Helper function for freeing queue nodes */
extern Customer* nextNode(Customer* C)
{
	free(C);
	return C->nextNode;

}



