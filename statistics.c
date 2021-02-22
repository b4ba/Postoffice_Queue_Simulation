#include "statistics.h"

/* Function to print the single statistics if simulation is run once in output file */
extern void printSingleStatistics(FILE *outputFile, STATISTICS *stats)
{
   fprintf(outputFile, "\n");
   fprintf(outputFile, "Time interval: %d\n", stats->timeInterval);
   fprintf(outputFile, "Customers currently being served: %d\n", stats->customersBeingServed);
   fprintf(outputFile, "Customers currently in queue:     %d\n", stats->customersInQueue);
   fprintf(outputFile, "Number of fulfilled customers:    %d\n", stats->fulfilledCustomer);
   fprintf(outputFile, "Number of unfulfilled customers:  %d\n", stats->unfulfilledCustomer);
   fprintf(outputFile, "Number of timed-out customers:    %d\n", stats->timedOutCustomer);
   fprintf(outputFile, "\n");
}

/* Function to print the average statistics of simulations in output file */
extern void printAverageStatistics(FILE *outputFile, AVERAGE_STATISTICS  *stats)
{
   fprintf(outputFile, "\n");
   fprintf(outputFile, "Average across all %d simulations\n", stats->totalNumSimulations - 1);
   fprintf(outputFile, "Average number of fulfilled customers:    %.2f\n", 1.00 * stats->totalFulfilledCustomers / stats->totalNumSimulations);
   fprintf(outputFile, "Average number of unfulfilled customers:  %.2f\n", 1.00 * stats->totalUnfulfilledCustomers / stats->totalNumSimulations);
   fprintf(outputFile, "Average number of timed out customers:    %.2f\n", 1.00 * stats->totalTimedOutCustomers / stats->totalNumSimulations);
   fprintf(outputFile, "Average wait time per customer:           %.3f\n", 1.00 * stats->averageWaitTime / stats->totalNumSimulations);
   fprintf(outputFile, "Average time from closing to completion:  %.2f\n", 1.00 * stats->totalClosingToCompletion / stats->totalNumSimulations);
   fprintf(outputFile, "\n");
}
