/****************************************************
 * Author: Alex Brisnehan                           *
 * Date: 10/27/2014                                 *
 * File: EventScheduler.h                           *
 * Use: The header file for the EventScheduler class*
 ****************************************************/
	
#ifndef _EVENT_SCHEDULER_
#define _EVENT_SCHEDULER_

#include <vector>
#include <queue>
#include <mutex>
#include "ThreadPool.h"

typedef void (*function_pointer)(void*);

struct Event
{
    function_pointer funk;
    void* arg;
    timeval trigTime;
    int id;
};

class CompareEvent {
public:
    bool operator()(Event& e1, Event& e2) { 
        if (e2.trigTime.tv_sec == e1.trigTime.tv_sec && e2.trigTime.tv_usec < e1.trigTime.tv_usec) return true;
        if (e2.trigTime.tv_sec < e1.trigTime.tv_sec) return true;
        return false;
    };
};

class EventScheduler
{
public:
    EventScheduler();
    EventScheduler(size_t maxEvents);
    ~EventScheduler();
    int eventSchedule(void evFunction(void *), void *arg, int timeout);
    void eventCancel(int eventId);

private:
    static void planEvent(void* arg);
    bool done;
    int eventID;
    size_t eventsMaximum;
    std::vector<int> eventCancelled;
    std::mutex eventMutex;
    std::priority_queue<Event, std::vector<Event>, CompareEvent> eventQueue;
    ThreadPool* eventPool;
    timeval tick;
};

#endif