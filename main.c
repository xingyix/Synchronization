/*
 * Weiling Zheng(Lydia), 301294091.
 *
 * main.c
 *
 *
 */

#include "main.h"
#include <pthread.h>

/*
*testing variables
*pthread_mutex_t count_mutex;
*volatile pthread_spinlock_t count_spinlock;
*my_spinlock_t count_spinTAS;
*my_spinlock_t count_spinTTAS;
*my_mutex_t count_mutexTTAS;
*my_queuelock_t count_myQueueLock;
*/

void *fnC()
{
    int i;
    for(i=0;i<1000000;i++)
    {   
        c++;
    }   
}

// Mutex lock Test
void *pthreadMutexTest()
{
    int i;
	int j;
	int k;
	
	int localCount = 0;
	
    for(i=0;i<numItterations;i++){	
		for(j=0;j<workOutsideCS;j++){/*How much work is done outside the CS*/
			localCount++;
		}
		
		pthread_mutex_lock(&count_mutex);
		for(k=0;k<workInsideCS;k++){/*How much work is done inside the CS*/
			c++;
		}
		pthread_mutex_unlock(&count_mutex);    
    }   
}

//Spinlock Test
void *pthreadSpinlockTest()
{
    int i;
	int j;
	int k;
	
	int localCount = 0;
	
    for(i=0;i<numItterations;i++){	
		for(j=0;j<workOutsideCS;j++){/*How much work is done outside the CS*/
			localCount++;
		}
		
		//critical section
		pthread_spin_lock(&count_spinlock);
		for(k=0;k<workInsideCS;k++){/*How much work is done inside the CS*/
			c++;
		}
		pthread_spin_unlock(&count_spinlock);    
    }   
}

//Spinlock test-and-set Test
void *mySpinLockTAS()
{
    int i;
	int j;
	int k;
	
	int localCount = 0;
	
    for(i=0;i<numItterations;i++){	
		for(j=0;j<workOutsideCS;j++){/*How much work is done outside the CS*/
			localCount++;
		}
		
		my_spinlock_lockTAS(&count_spinTAS);
		for(k=0;k<workInsideCS;k++){/*How much work is done inside the CS*/
			c++;
		}
		my_spinlock_unlock(&count_spinTAS);    
    }   
}

//Spinlock test test-and-set Test
void *mySpinLockTTAS()
{
    int i;
	int j;
	int k;
	
	int localCount = 0;
	
    for(i=0;i<numItterations;i++){	
		for(j=0;j<workOutsideCS;j++){/*How much work is done outside the CS*/
			localCount++;
		}
		
		my_spinlock_lockTTAS(&count_spinTTAS);
		for(k=0;k<workInsideCS;k++){/*How much work is done inside the CS*/
			c++;
		}
		my_spinlock_unlock(&count_spinTTAS);    
    }   
}

//Exponential Backoff Lock Test
void *myMutexTTAS()
{
    int i;
	int j;
	int k;

	int localCount = 0;
	
    for(i=0;i<numItterations;i++){	
		for(j=0;j<workOutsideCS;j++){/*How much work is done outside the CS*/
			localCount++;
		}

		my_mutex_lock(&count_mutexTTAS);
		for(k=0;k<workInsideCS;k++){/*How much work is done inside the CS*/
			c++;
		}
		my_mutex_unlock(&count_mutexTTAS);    
    }   
}


//Queue(ticket)Lock Test
void *myQueueLock()
{
    int i;
	int j;
	int k;

	int localCount = 0;
	
    for(i=0;i<numItterations;i++){	
		for(j=0;j<workOutsideCS;j++){/*How much work is done outside the CS*/
			localCount++;
		}

		my_queuelock_lock(&count_myQueueLock);
		for(k=0;k<workInsideCS;k++){/*How much work is done inside the CS*/
			c++;
		}
		my_queuelock_unlock(&count_myQueueLock);    
    }   
}




int runTest(int testID)
{
	/*You must create all data structures and threads for you experiments*/
	if (testID == 0 || testID == 1 ) /*Pthread Mutex*/
	{
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer

		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++){
			if( rt=(pthread_create( threads+i, NULL, &pthreadMutexTest, NULL)) ){
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++){//Wait for all threads to finish
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("Threaded Run Pthread (Mutex) Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread Mutex time(ms): %llu\n",result/1000000);
	}

	if(testID == 0 || testID == 2){ /*Pthread Spinlock*/
		/*Pthread Spinlock goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		pthread_spin_init(&count_spinlock, 0); // initialize the lock to an unlocked state

		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++){
			if( rt=(pthread_create( threads+i, NULL, &pthreadSpinlockTest, NULL)) ){
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++){//Wait for all threads to finish
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("----------------------------------------------------\n");
		printf("Threaded Run Pthread (Spinlock) Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread Spinlock time(ms): %llu\n",result/1000000);
	}

	if(testID == 0 || testID == 3){ /*MySpinlockTAS*/
	/* mySpinlock TAS goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		my_spinlock_init(&count_spinTAS);

		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++){
			if( rt=(pthread_create( threads+i, NULL, &mySpinLockTAS, NULL)) ){
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++){//Wait for all threads to finish
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("----------------------------------------------------\n");
		printf("Threaded Run Pthread (SpinlockTAS) Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread SpinlockTAS time(ms): %llu\n",result/1000000);
	}

	/*....you must implement the other tests....*/
	if(testID == 0 || testID == 4){ /*mySpinLockTTAS*/
	/* mySpinlock TTAS goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		my_spinlock_init(&count_spinTTAS);

		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++){
			if( rt=(pthread_create( threads+i, NULL, &mySpinLockTTAS, NULL)) ){
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}
		
		for(i=0;i<numThreads;i++){//Wait for all threads to finish
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("----------------------------------------------------\n");
		printf("Threaded Run Pthread (SpinlockTTAS) Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread SpinlockTTAS time(ms): %llu\n",result/1000000);
	}

	if(testID == 0 || testID == 5){ /*myMutexTAS*/
	/* myMutexTAS TTAS goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		my_mutex_init(&count_mutexTTAS);

		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++){
			if( rt=(pthread_create( threads+i, NULL, &myMutexTTAS, NULL)) ){
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}

		for(i=0;i<numThreads;i++){//Wait for all threads to finish
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("----------------------------------------------------\n");
		printf("Threaded Run Pthread (MutexTTAS) Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread MutexTTAS time(ms): %llu\n",result/1000000);
	}


	if(testID == 0 || testID == 6){ /*myQueueLock*/
	/* myQueueLock goes here*/
		c=0;
		struct timespec start;
		struct timespec stop;
		unsigned long long result; //64 bit integer
		my_queuelock_init(&count_myQueueLock);

		pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);	
		int i;
		int rt;

		clock_gettime(CLOCK_MONOTONIC, &start);
		for(i=0;i<numThreads;i++){
			if( rt=(pthread_create( threads+i, NULL, &myQueueLock, NULL)) ){
				printf("Thread creation failed: %d\n", rt);
				return -1;	
			}
		}

		for(i=0;i<numThreads;i++){//Wait for all threads to finish
			 pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_MONOTONIC, &stop);

		printf("----------------------------------------------------\n");
		printf("Threaded Run Pthread (myQueueLock) Total Count: %llu\n", c);
		result=timespecDiff(&stop,&start);
		printf("Pthread myQueueLock time(ms): %llu\n",result/1000000);
	}


	return 0;
}




int testAndSetExample(){
	volatile long test = 0; //Test is set to 0
	printf("Test before atomic OP:%lo\n",test);
	tas(&test);
	printf("Test after atomic OP:%lo\n",test);
}

int processInput(int argc, char *argv[])
{

	/*testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock*/
	/*You must write how to parse input from the command line here, your software should default to the values given below if no input is given*/
	
	numThreads=4;
	numItterations=1000000;
	testID=0;
	workOutsideCS=0;
	workInsideCS=1;
	int i; //index

	//initialization --> argc will always be at least 1
	if (argc > 1){
		for(i = 0; i < argc; i++){
			if(strcmp(argv[i], "-t") == 0){
				numThreads = atoi(argv[i+1]);
				if(atoi(argv[i+1]) == 0 && strcmp(argv[i+1], "0") != 0)
					printf("***ERROR: Wrong threads input!\n");
			}

			if(strcmp(argv[i], "-i") == 0){
				numItterations = atoi(argv[i+1]);
				if(atoi(argv[i+1]) == 0 && strcmp(argv[i+1], "0") != 0)
					printf("***ERROR: Wrong Itterations input!\n");
			}

			if(strcmp(argv[i], "-d") == 0){
				testID = atoi(argv[i+1]);
				if(atoi(argv[i+1]) == 0 && strcmp(argv[i+1], "0") != 0)
					printf("***ERROR: Wrong OperationsOutsideCS input!\n");
			}

			if(strcmp(argv[i], "-o") == 0){
				workOutsideCS = atoi(argv[i+1]);
				if(atoi(argv[i+1]) == 0 && strcmp(argv[i+1], "0") != 0)
					printf("***ERROR: Wrong OperationsInsideCS input!\n");
			}

			if(strcmp(argv[i], "-c") == 0){
				workInsideCS = atoi(argv[i+1]);
				if(atoi(argv[i+1]) == 0 && strcmp(argv[i+1], "0") != 0)
					printf("***ERROR: Wrong testid input!\n");
			}
		}
	}
	else if (argc < 1)
		printf("The command had no other arguments\n");

	//print out the process input of each element
	printf("\nnumThreads: %d\n", numThreads);
	printf("numItterations: %d\n", numItterations);
	printf("workOutsideCS: %d\n", workOutsideCS);
	printf("workInsideCS: %d\n", workInsideCS);
	printf("testID: %d\n\n", testID);
	return 0;
}


int main(int argc, char *argv[])
{
	printf("Usage of: %s -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid\n", argv[0]);
	printf("testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTTAS, 6=myQueueLock, \n\n");	
	
	//testAndSetExample(); //Uncomment this line to see how to use TAS
	
	processInput(argc,argv);
	runTest(testID);
	return 0;

}
