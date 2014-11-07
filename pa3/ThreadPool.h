/************************************************
 * Author: Alex Brisnehan                       *
 * Date: 10/27/2014                             *
 * File: ThreadPool.h                           *
 * Use: The header file for the ThreadPool class*
 ************************************************/


#include <iostream>
#include <map>
#include <mutex>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#ifndef _THREAD_POOL_
#define _THREAD_POOL_
typedef void (*function_pointer)(void*);

class ThreadPool
{
public:
	ThreadPool();
    ThreadPool(size_t threadCount);
    ~ThreadPool();
    int dispatch_thread(void dispatch_function(void*), void *arg);
    bool thread_avail();
    int getCount();

private:
	void execThread();
    static void* threadInit(void* arg);
    pthread_t* threads;
    size_t totalThread;
    std::map<pthread_t, void*> argue;
    std::map<pthread_t, bool> avail;
    std::map<pthread_t, function_pointer> funk;
    std::map<pthread_t, sem_t> semiTruck;
};

#endif