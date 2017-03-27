/*
 * Weiling Zheng(Lydia), 301294091.
 *
 *
 * sync.h
 */
#ifndef _my_SYNC1_H_
#define _my_SYNC1_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "atomic_ops.h"

/*Spinlock Starts here*/

struct my_spinlock_struct {
  /* FILL ME IN! */
	pthread_t tid;
	volatile unsigned long status; // status of lock (locked or unlocked)
	int count; //count of the lock
};

typedef struct my_spinlock_struct my_spinlock_t;

int my_spinlock_init(my_spinlock_t *lock);
int my_spinlock_destroy(my_spinlock_t *lock);
int my_spinlock_unlock(my_spinlock_t *lock);

int my_spinlock_lockTAS(my_spinlock_t *lock);
int my_spinlock_lockTTAS(my_spinlock_t *lock);
int my_spinlock_trylock(my_spinlock_t *lock);

/*Mutexlock Starts here */
struct my_mutex_struct {
    /* FILL ME IN! */
    pthread_t tid;
	volatile unsigned long status;
	int count;
	int delay;
};

typedef struct my_mutex_struct my_mutex_t;


int my_mutex_init(my_mutex_t *lock);
int my_mutex_unlock(my_mutex_t *lock);
int my_mutex_destroy(my_mutex_t *lock);

int my_mutex_lock(my_mutex_t *lock);
int my_mutex_trylock(my_mutex_t *lock);


/*queuelock Starts here*/

struct my_queuelock_struct {
  /* FILL ME IN! */
	pthread_t tid;
	
	//queue ticket
	volatile unsigned long next_ticket;//the thread's position in queue
	//dequeue ticket
	volatile unsigned long now_serving;// the queue position, what now has the lock
	int count;
};

typedef struct my_queuelock_struct my_queuelock_t;

int my_queuelock_init(my_queuelock_t *lock);
int my_queuelock_destroy(my_queuelock_t *lock);
int my_queuelock_unlock(my_queuelock_t *lock);

int my_queuelock_lock(my_queuelock_t *lock);
int my_queuelock_trylock(my_queuelock_t *lock);


#endif
