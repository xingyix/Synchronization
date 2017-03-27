/*
 * Weiling Zheng(Lydia), 301294091.
 *
 * sync.c
 *
 *
 */
#define _REENTRANT
#define MIN(X,Y) (X<Y ? X:Y)
#define MAX(X,Y) (X>Y ? X:Y)
#define MIN_DELAY 100
#define MAX_DELAY 1000000

#include "sync.h"

/*
 * Spinlock routines
 */
int my_spinlock_init(my_spinlock_t *lock){
	// initialize the status & tid & count to 0
	lock->status = 0;
	lock->tid = 0;
	lock->count = 0;
	return 0;
}

int my_spinlock_destroy(my_spinlock_t *lock){
	return 0;
}

int my_spinlock_unlock(my_spinlock_t *lock){
	pthread_t myself = pthread_self();	
	if(lock->tid == myself && (--lock->count) == 0)
	{
		lock->tid = 0;
		lock->status = 0;
	}

	return 0;
}

int my_spinlock_lockTAS(my_spinlock_t *lock){
	pthread_t myself = pthread_self(); // contain ID of the calling thread

	if(lock->tid == myself)
		lock->count++;
	else{
		//The calling process obtains the lock
		//if the old value was 0
		//otherwise while-loop spins waiting to acquire the lock
		while(1){
			while(tas(&lock->status) == 1)// when lock is not free(locked)
				 ;// spin here
			lock->count++;
			lock->tid = myself;
			break;
		}
	}

	return 0;
}

int my_spinlock_lockTTAS(my_spinlock_t *lock){
	pthread_t myself = pthread_self(); // contain ID of the calling thread


	if(lock->tid==myself){
		lock->count++;
	}
	else{
		while(1){
			while(lock->status); //check the status of the lock before attemption to acquire it

			if(tas(&lock->status) == 0){ //wait until the lock is free
				lock->count++;
				lock->tid = myself;
				break; // then acquire it
			}
		}
	}

	return 0;
}

int my_spinlock_trylock(my_spinlock_t *lock){
	/*
	spin_trylock() does not spin but 
	returns non-zero if it acquires the spinlock on the first try 
	or 0 if not.
	*/
	pthread_t myself = pthread_self(); // contain ID of the calling thread
	if(tas(&lock->status) == 0){
		lock->count++;
		lock->tid = myself;
	}
	return 0;
}



/*
 * Mutex routines
 */

int my_mutex_init(my_mutex_t *lock){
	lock->status = 0;
	lock->tid = 0;
	lock->count = 0;
	lock->delay = rand()%MIN_DELAY; // delay in second
	return 0;
}

int my_mutex_destroy(my_mutex_t *lock){
	return 0;
}

int my_mutex_unlock(my_mutex_t *lock){
	pthread_t myself = pthread_self();
	if(lock->tid == myself && (--lock->count) == 0)
	{
		lock->tid = 0;
		lock->status = 0;
	}

	return 0;
}

int my_mutex_lock(my_mutex_t *lock){
	pthread_t myself = pthread_self(); // contain ID of the calling thread
	//lock->delay = rand()%MIN_DELAY;

	if(lock->tid != myself){
		while(tas(&lock->status) == 1){ // if it is unlocked
			usleep(lock->delay);// suspend execution for microsecond intervals
			//Each time the thread tries and fails to get the lock
			//it doubles the expected back-off time, up to a fixed max.
			if(lock->delay >= MAX_DELAY)
				lock->delay = MAX(MAX_DELAY, lock->delay % MIN_DELAY);
			else
				lock->delay = MIN(MAX_DELAY, 2 * lock->delay);// if delay is less than MAX_DELAY, delay*=2
		}
		lock->count++;
		lock->delay = rand()%MIN_DELAY;// wait random duration before retry
		lock->tid = myself;
	}	
	else lock->count++;

	return 0;
}

int my_mutex_trylock(my_mutex_t *lock){
	/*
	mutex_trylock() does not suspend your task but 
	returns non-zero if it could lock the mutex on the first try 
	or 0 if not.
	*/
	pthread_t myself = pthread_self(); // contain ID of the calling thread
	if(tas(&lock->status) == 0){
		lock->count++;
		lock->tid = myself;
	}
	return 0;
}



/*
 * Queue Lock
 */

int my_queuelock_init(my_queuelock_t *lock){
	lock->tid = 0;
	lock->count = 0;
	lock->next_ticket = 0;
	lock->now_serving = 0;
	return 0;
}

int my_queuelock_destroy(my_queuelock_t *lock){
	return 0;
}

int my_queuelock_unlock(my_queuelock_t *lock){
	if(lock->tid == pthread_self() && --lock->count==0){
		lock->tid = 0;
		lock->now_serving++;
	}
	return 0;
}

int my_queuelock_lock(my_queuelock_t *lock){
	pthread_t myself = pthread_self(); // contain ID of the calling thread

	//if the lock is acquired by itself
	if(myself == lock->tid){
		lock->count++;
	}
	else{
		while(1){
			// use another variable to store next_ticket number
			// which can be used later
			long next = lock->next_ticket;
			long ticket = cas(&lock->next_ticket, next, next+1);

			if(ticket == next){
				while(lock->now_serving != ticket) {}

				lock->tid = myself;
				lock->count++;
				break;
			}
		}
	}

	return 0;
}

int my_queuelock_trylock(my_queuelock_t *lock){
	//Check if the lock is free (now served = current ticket).
	pthread_t myself = pthread_self();

	if(myself == lock->tid){
		lock->count++;
	}
	else{
		// use another variable to store next_ticket number
		// which can be used later
		long next = lock->next_ticket;
		long ticket = cas(&lock->next_ticket, next, next+1);

		if(ticket == next){
			if(lock->now_serving == ticket){
				lock->tid = myself;
				lock->count++;
			}
		}
	}
	return 0;
}

