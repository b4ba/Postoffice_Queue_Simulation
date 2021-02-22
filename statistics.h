#ifndef __STATISTICS_H
#define __STATISTICS_H
#include <stdio.h>
/* Initialising structures */
struct statistics
{
	int timeInterval;
        
	int customersInQueue; /* Customers currently in queue (should equal count) */
        int customersBeingServed; /* Customers in the service points */
        
	int fulfilledCustomer; /* Customers who finished being served at postoffice */
        int unfulfilledCustomer; /* Customers who arrived and queue was full */
        int timedOutCustomer; /* Customers who arrived and left queue after waiting too long */
        
	int totalWaitTime;
        int closingTimeToCompletion;
        int totalServeTime;


};

typedef struct statistics STATISTICS;

struct averageStatistics
{
   int totalFulfilledCustomers;
   int totalUnfulfilledCustomers;
   int totalTimedOutCustomers;
   int averageWaitTime;
   int totalClosingToCompletion;
   int totalNumSimulations;
};

typedef struct averageStatistics AVERAGE_STATISTICS;


/* Function prototypes */
extern void printSingleStatistics(FILE *, STATISTICS *);
extern void printAverageStatistics(FILE *, AVERAGE_STATISTICS  *);
#endif
