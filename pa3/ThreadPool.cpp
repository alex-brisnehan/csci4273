/************************************************
 * Author: Alex Brisnehan                       *
 * Date: 10/27/2014                             *
 * File: ThreadPool.cpp                         *
 * Use: The source file for the ThreadPool class*
 ************************************************/

#include <iostream>
#include <queue>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "ThreadPool.h"

using namespace std;

/* Default Constructor 
 * Set initial count to 10
 * Create Thread pool usng pthread_create
 * Return the pool */

ThreadPool::ThreadPool()
{
    int err, i;
    sem_t mutex;
    totalThread = 10;
    threads = new pthread_t[10];

    for (i = 0; i < totalThread; i++) {
        err = sem_init(&mutex, 0, 1);
        if (err < 0) {
            cout << "ERROR: " << strerror(errno) << endl;
            exit(1);
        }

        sem_wait(&mutex);

        pthread_t threadID;
        err = pthread_create(&threadID, NULL, ThreadPool::threadInit, (void*) this);
        if (err != 0) {
            cout << "Failure on pthread_create" << err << endl;
            exit(1);
        }
        semiTruck[threadID] = mutex;
        avail[threadID] = true;
        threads[i] = threadID;
    }
}

/* Constructor with value
 * Set thread count to parameter
 * Use pthread_create to create pool
 * Return thread pool */

ThreadPool::ThreadPool(size_t threadCount)
{
    int err, i;
    sem_t mutex;
    totalThread = threadCount;
    threads = new pthread_t[threadCount];

    for (i = 0; i < totalThread; i++) {
        err = sem_init(&mutex, 0, 1);
        if (err < 0) {
            cout << "ERROR: " << strerror(errno) << endl;
            exit(1);
        }

        sem_wait(&mutex);

        pthread_t threadID;
        err = pthread_create(&threadID, NULL, ThreadPool::threadInit, (void*) this);
        if (err != 0) {
            cout << "Failure on pthread_create" << err << endl;
            exit(1);
        }
        semiTruck[threadID] = mutex;
        avail[threadID] = true;
        threads[i] = threadID;
    }
}

/*DESTRUCTOR*/
ThreadPool::~ThreadPool()
{
    delete[] threads;
}

/* Start running the thread/get it ready to run */
int ThreadPool::dispatch_thread(void dispatch_function(void*), void *arg)
{
    int i;
    for (i = 0; i < totalThread; i++) {
        pthread_t threadID = threads[i];
        if (avail[threadID]) {
            argue[threadID] = arg;
            avail[threadID] = false;
            funk[threadID] = dispatch_function;
            sem_post(&(semiTruck[threadID]));
            return 0;
        }
    }
    return -1;
}

/* Is there an open thread to use? */
bool ThreadPool::thread_avail()
{
    int i;
    for (i = 0; i < totalThread; i++) {
        if (avail[threads[i]]) {
            return true;
        }
    }
    return false;
}

/* Getter for threadCount */
int ThreadPool::getCount()
{
   return totalThread;
}

/* Execute the thread */
void ThreadPool::execThread()
{
    pthread_t threadID = pthread_self();
    while (true) {
        sem_wait(&semiTruck[threadID]);
        (*(funk[threadID]))(argue[threadID]);
        avail[threadID] = true;
    }
}

/* Initialize a thread in the threadpool and then execute it */
void* ThreadPool::threadInit(void* arg)
{
    ThreadPool* tp = (ThreadPool*) arg;
    tp->execThread();
    return NULL;
}
