#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include "sync.h"

long long c = 0;
int numThreads;
int numItterations;
int workOutsideCS;
int workInsideCS;
int testID;

// setting the testing variables
pthread_mutex_t count_mutex;
volatile pthread_spinlock_t count_spinlock;
my_spinlock_t count_spinTAS;
my_spinlock_t count_spinTTAS;
my_mutex_t count_mutexTTAS;
my_queuelock_t count_myQueueLock;


unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

