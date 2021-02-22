#include "simQ.h"

/* Main method */
int main(int argc, char **argv) 
{
    /* Setting up the simulation */
    validateArgs(argc,argv);
    char *inputFile = argv[1];
    int numSims = atoi(argv[2]);
    char *outputFile = argv[3];
	
    FILE *fileOut = NULL;
    if((fileOut = fopen(outputFile, "w")) == NULL )
    {
        fprintf(stderr, "Cannot write to output file %s: Error: %s", outputFile, strerror(errno));
    }

    INPUT fileInput;
    readFile(inputFile, &fileInput, fileOut);

    QUEUE *postOfficeQueue = InitialiseQueue();	
	
    Customer *servicePoints[fileInput.numServicePoints];
    memset(servicePoints, 0, fileInput.numServicePoints*sizeof(Customer*));

    gsl_rng *randomNum  = randomGenerator();
	
    STATISTICS *stats = NULL;
    stats = (STATISTICS *) malloc(sizeof(STATISTICS));

    AVERAGE_STATISTICS *averageStats = NULL;
    averageStats = (AVERAGE_STATISTICS *) malloc(sizeof(AVERAGE_STATISTICS));

    /* Start of simulation */	
    for(averageStats->totalNumSimulations; averageStats->totalNumSimulations < numSims; averageStats->totalNumSimulations++)
    {
        for( stats->timeInterval = 0; stats->timeInterval <= fileInput.closingTime; stats->timeInterval++)
        {	
	/* - check the customers being served to see if they are finished so they can leave */
            int customerServed;
	    for(customerServed = 0; customerServed < fileInput.numServicePoints; customerServed++)
	    {
	        if(servicePoints[customerServed])
		{
	            servicePoints[customerServed]->servingTime--;
		    if(servicePoints[customerServed]->servingTime == 0)
		    {
                        free(servicePoints[customerServed]);
			servicePoints[customerServed] = NULL;
					
			stats->customersBeingServed--;
			stats->fulfilledCustomer++;
		    }
		}
		else{
		        if(postOfficeQueue->Head)
			{
			    /* - the next customer in the queue starts to get served */
			    Customer* nextCustomer = dequeue(postOfficeQueue);
			    stats->totalWaitTime += (nextCustomer->timeSpentWaiting - nextCustomer->toleranceLimit);
			    servicePoints[customerServed] = nextCustomer;
			    stats->customersBeingServed++;
			    stats->customersInQueue--;
			}
		    }
            }

            /* - check all customers in the queue to see if they reached their waiting limit */
            if(postOfficeQueue->Count > 0)
            {	
	        int customersLeft = removeFromQueue(postOfficeQueue);	
		stats->timedOutCustomer+=customersLeft;
		stats->customersInQueue-=customersLeft;
	    }
	    /* Customers arrive at random intervals and join the back of the queue if not full */
            int arrival = (int)gsl_ran_flat(randomNum,1, fileInput.averageNewCustomerPerInterval * 2);
	    for(arrival; arrival > 0; arrival--)
	    {
	        if(postOfficeQueue->Count == fileInput.maxQueueLength)
		{
                    stats->unfulfilledCustomer++;	
				
		}else
		{	
		    enqueue(postOfficeQueue, createCustomer(&randomNum));
		    stats->customersInQueue++;
		}
	    }
	
	    if( numSims == 1 )
            {
                printSingleStatistics(fileOut, stats);
            }
	
	}

        if( numSims == 1 )
   	{
      	    fprintf(fileOut, "------------ CLOSING TIME ------------\n");
        }

        /* As soon as the simulation ends, serve the remaining customers in the queue */
        while(postOfficeQueue->Count > 0 || stats->customersBeingServed > 0)
	{
            int customerServedAfter; 
	    for(customerServedAfter = 0; customerServedAfter < fileInput.numServicePoints; customerServedAfter++)
            {
                if(servicePoints[customerServedAfter])
               	{
                    servicePoints[customerServedAfter]->servingTime--;
                    if(servicePoints[customerServedAfter]->servingTime == 0)
                    {
                        free(servicePoints[customerServedAfter]);
                        servicePoints[customerServedAfter] = NULL;

                        stats->fulfilledCustomer++;
	                stats->customersBeingServed--;
                    }
                 }
                 else{
                         if(postOfficeQueue->Head)
                         {
                             Customer* nextCustomer = dequeue(postOfficeQueue);
	   		     servicePoints[customerServedAfter] = nextCustomer;
			     stats->customersBeingServed++;
                             stats->customersInQueue--;
			 }
                     }
          }
	  if( numSims == 1 )
          {
              printSingleStatistics(fileOut, stats);
          }
	      stats->timeInterval++;
	      stats->closingTimeToCompletion++;
        }
    }
	
    if( numSims == 1 )
    {
        fprintf(fileOut, "----Time from closing until all customers were served: %d intervals----\n", stats->closingTimeToCompletion);
      	fprintf(fileOut, "----Average wait time of fulfilled customers: %.2f intervals----\n", 1.0 * stats->totalWaitTime / stats->fulfilledCustomer);
    }else
    {
        /* Gathering the total statistics in order to find the average values */
        averageStats->totalNumSimulations++;
	averageStats->averageWaitTime += ((1.0 * stats->totalWaitTime / stats->fulfilledCustomer));
	averageStats->totalClosingToCompletion += stats->closingTimeToCompletion;
	averageStats->totalFulfilledCustomers += stats->fulfilledCustomer;
	averageStats->totalUnfulfilledCustomers += stats->unfulfilledCustomer;
	averageStats->totalTimedOutCustomers += stats->timedOutCustomer;
    }

    if(numSims > 1)
    {
        printAverageStatistics(fileOut, averageStats);
    }

    /* Deallocating memory at the end of the simulation */
    int i;
    for(i = 0; i < fileInput.numServicePoints; i++){
        free(servicePoints[i]);
    }
    fclose(fileOut);
    gsl_rng_free(randomNum);	
    free(postOfficeQueue);
    free(stats);
    free(averageStats);
    printf("\nSuccessfully completed simulation, output can be found in %s\n", outputFile);
    return EXIT_SUCCESS;
}

/* Function for adding a customer to the queue */
Customer* createCustomer(gsl_rng ** randomNum)
{
	Customer *newCustomer = NULL;
	newCustomer = (Customer *)malloc(sizeof(Customer)); /* Dynamically allocating memory to customer */
	if(newCustomer == NULL)
	{
		fprintf(stderr, "Memory allocation error for newCustomer: %d: %s\n", errno, strerror(errno));
		exit(-1);
	}
	/* Randomly generating numbers and assigning to customer parameters */
	int toleranceLimit = abs(gsl_ran_poisson(*randomNum,8)) + 1;
	newCustomer->toleranceLimit = toleranceLimit;
	newCustomer->timeSpentWaiting = toleranceLimit;
	newCustomer->servingTime = abs(gsl_ran_poisson(*randomNum,5)) + 1;
	newCustomer->nextNode = NULL;
	newCustomer->previousNode = NULL;
	return newCustomer;
}

/* Function to setup the random number generator from GSL library */

gsl_rng * randomGenerator(INPUT *I)
{
	const gsl_rng_type *T;
 	gsl_rng            *r;
	
	/* create random number generator */
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);

	/* seed it - equivalent of srand(time(0)) */
	gsl_rng_set(r, time(0));
	return r;
}

/* A very simple way to validate arguments */
void validateArgs(int size, char **args) {
	char *inputFile = strchr(args[1], '.');
	int numSimulation = atoi(args[2]);
	char *outputFile = strchr(args[3], '.');
	char err = 0;

	if(size < 4 || size > 4){
		printf("Error: Invalid number of arguments\n");
		err = 1;
	}

	if(inputFile && strcmp(inputFile, ".txt")) {
		printf("Error: Input file not found or invalid type\n");
		err = 1;
	}

	if(numSimulation < 1 || numSimulation > 100000) {
		printf("Error: Invalid Simulation number\n");
		err = 1;
	}
	
	if(outputFile && strcmp(outputFile, ".txt")) {
		printf("Error: Output file not found or invalid type\n");
               	err = 1;
        } 
	
	
	if (err) {
		exit(1);
	}
}
