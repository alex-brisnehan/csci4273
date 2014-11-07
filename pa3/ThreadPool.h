/************************************************
 * Author: Alex Brisnehan                       *
 * Date: 10/27/2014                             *
 * File: ThreadPool.h                           *
 * Use: The header file for the ThreadPool class*
 ************************************************/

#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <map>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <mutex>
#include <iostream>

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
    size_t totalThread;
    pthread_t* threads;;
    std::map<pthread_t, sem_t> semiTruck;
    std::map<pthread_t, bool> avail;
    std::map<pthread_t, function_pointer> funk;
    std::map<pthread_t, void*> argue;


    void execThread();
    static void* threadInit(void* arg);
};

#endif