# ss-hw5
Introduction to Multithreading

- The importance of multithreading in modern applications.


- How mutexes and condition variables help in synchronizing threads.
Mutexes and condition variables create a locking structure and inter-thread communication system. A mutex locks a resource so that modifications on that resource happen atomically. Condition variables allow threads to signal one another when there is work available to be done (i.e. a producer-consumer model sharing a queue with a mutex. The producer will notify the consumer who is waiting for the queue to be populated before performing some work on the queue (the shared resource)).

- Common pitfalls in multithreading, such as deadlocks, and how to avoid them.
Having two or more locks and requiring that a thread have both in order to proceed without paying attention to the ordering of the acquisition of the locks.