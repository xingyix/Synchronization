Weiling Zheng (Lydia) 
301294091  weilingz@sfu.ca

Instructions on running the function are listed as follows:

1. Type 'make' to generate the executable file
2. Type './mylocks -t 4 -i 1000000 -o 1 -c 1 -d 0'
	- which means there are 4 threads, 1000000 iterations, 1 opeartion outside critical section, 1 operation inside critical section and test all the locks.
	- You can change the number to test other cases.
3. Type 'make clean' to recompile the file

The output samples are presented as follows:

1.

weilingz@asb9820u-b04:~/Desktop/base-a3$ ./mylocks -t 8 -i 1000000 -o 1 -c 1 -d 0
Usage of: ./mylocks -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid
testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTTAS, 6=myQueueLock, 


numThreads: 8
numItterations: 1000000
workOutsideCS: 1
workInsideCS: 1
testID: 0

Threaded Run Pthread (Mutex) Total Count: 8000000
Pthread Mutex time(ms): 933
----------------------------------------------------
Threaded Run Pthread (Spinlock) Total Count: 8000000
Pthread Spinlock time(ms): 746
----------------------------------------------------
Threaded Run Pthread (SpinlockTAS) Total Count: 8000000
Pthread SpinlockTAS time(ms): 5360
----------------------------------------------------
Threaded Run Pthread (SpinlockTTAS) Total Count: 8000000
Pthread SpinlockTTAS time(ms): 1373
----------------------------------------------------
Threaded Run Pthread (MutexTTAS) Total Count: 8000000
Pthread MutexTTAS time(ms): 654
----------------------------------------------------
Threaded Run Pthread (myQueueLock) Total Count: 8000000
Pthread myQueueLock time(ms): 1077

2. (default) Output example

weilingz@asb9820u-b04:~/Desktop/base-a3$ ./mylocks
Usage of: ./mylocks -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid
testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTTAS, 6=myQueueLock, 


numThreads: 4
numItterations: 1000000
workOutsideCS: 0
workInsideCS: 1
testID: 0

Threaded Run Pthread (Mutex) Total Count: 4000000
Pthread Mutex time(ms): 893
----------------------------------------------------
Threaded Run Pthread (Spinlock) Total Count: 4000000
Pthread Spinlock time(ms): 304
----------------------------------------------------
Threaded Run Pthread (SpinlockTAS) Total Count: 4000000
Pthread SpinlockTAS time(ms): 1096
----------------------------------------------------
Threaded Run Pthread (SpinlockTTAS) Total Count: 4000000
Pthread SpinlockTTAS time(ms): 373
----------------------------------------------------
Threaded Run Pthread (MutexTTAS) Total Count: 4000000
Pthread MutexTTAS time(ms): 118
----------------------------------------------------
Threaded Run Pthread (myQueueLock) Total Count: 4000000
Pthread myQueueLock time(ms): 427
