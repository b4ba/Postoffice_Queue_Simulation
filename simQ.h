
#ifndef __SIMQ_H
#define __SIMQ_H

#include "Queue.h"
#include "statistics.h"
#include "input.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Defining non-queue function prototypes */
Customer* createCustomer(gsl_rng **);
gsl_rng * randomGenerator();
void validateArgs(int, char **);
#endif
