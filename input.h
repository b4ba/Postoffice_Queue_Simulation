#ifndef __INPUT_H
#define __INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Initialising structs */
struct fileInput
{
        int maxQueueLength;
        int numServicePoints;
        int closingTime;
        int averageNewCustomerPerInterval;
};

typedef struct fileInput INPUT;

/* Defining non-queue function prototypes */
INPUT * readFile(char *, INPUT *, FILE *);

#endif
