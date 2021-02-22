#include "input.h"

/* Function to read the parameters from the input file and store them in variables */
INPUT * readFile(char *fileName, INPUT *I, FILE *outputFile)
{
        char c[100];
        FILE *fp = NULL;
        if ((fp = fopen(fileName, "r")) == NULL)
        {
                fprintf(stderr, "Unable to open file and read parameters: %d: %s\n", errno, strerror(errno));
                exit(-1);
        }

        while( !feof(fp) )
        {
                fgets(c, 100, fp);
                if( c[0] != '#' && strlen(c) > 1 )
                {
                        sscanf(c, "maxQueueLength %d", &(I->maxQueueLength));
                        sscanf(c, "numServicePoints %d", &(I->numServicePoints));
                        sscanf(c, "closingTime %d", &(I->closingTime));
                        sscanf(c, "averageNewCustomersPerInterval %d", &(I->averageNewCustomerPerInterval));
                }
        }
        if(I->maxQueueLength == 0)
        {
                fprintf(stderr, "Parameter maxQueueLength wasn't identified\n");
        }
        if(I->numServicePoints == 0)
        {
                fprintf(stderr, "Parameter numServicePoints wasn't identified\n");
        }
        if(I->closingTime == 0)
        {
                fprintf(stderr, "Parameter closingTime wasn't identified\n");
        }
        if(I->averageNewCustomerPerInterval == 0)
        {
                fprintf(stderr, "Parameter averageNewCustomersPerInterval wasn't identified\n");
        }
        
	fprintf(outputFile, "----Parameters read from %s----\n", fileName);
   	fprintf(outputFile, "Max queue length:          %d\n",  I->maxQueueLength);
   	fprintf(outputFile, "Number of service points:  %d\n", I->numServicePoints);
   	fprintf(outputFile, "Closing time:              %d\n", I->closingTime);
   	fprintf(outputFile, "Average new customers per interval: %d\n", I->averageNewCustomerPerInterval);


	fclose(fp);
        return I;
}

