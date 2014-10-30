/************************************************
 * Author: Alex Brisnehan                       *
 * Date: 10/27/2014                             *
 * File: ThreadPool.h                           *
 * Use: The header file for the ThreadPool class*
 ************************************************/

#ifndef _THREADPOOL_
#define _THREADPOOL_

#include <pthread.h>
#include <string.h>

using namespace std;

class ThreadPool {
    public:
	ThreadPool(size_t threadCount);
	~ThreadPool();
	int dispatch_thread(void dispatch_function(void*), void *arg);
	bool thread_avail();
};

#endif

