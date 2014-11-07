/****************************************************
 * Author: Alex Brisnehan                           *
 * Date: 10/27/2014                                 *
 * File: EventScheduler.cpp                         *
 * Use: The source file for the EventScheduler class*
 ****************************************************/
#include "EventScheduler.h"
#include <pthread.h>
#include <string.h>
#include <iostream>
#include <sys/select.h>
#include <sys/time.h>

using namespace std;

EventScheduler::EventScheduler()
{
    eventsMaximum = 10;
    eventID = 0;
    eventPool = new ThreadPool();
}
EventScheduler::EventScheduler(size_t maxEvents)
{
    eventsMaximum = maxEvents;
    eventID = 0;
    eventPool = new ThreadPool(maxEvents);
}

EventScheduler::~EventScheduler()
{
    //while(1);
    delete eventPool;
}

int EventScheduler::eventSchedule(void evFunction(void *), void *arg, int timeout)
{
    int milli = timeout * 1000;
    int second = 0;
    while (milli >= 1000000) {
        milli -= 1000000;
        second++;
    }
    timeval eventTime= {second, milli};

    // create an event and push it on the queue
    Event e = {evFunction, arg, eventTime, eventID};
    eventMutex.lock();
    eventQueue.push(e);
    eventMutex.unlock();

    done = true;
    eventPool->dispatch_thread(planEvent, this);
    
    return eventID++;
}

void EventScheduler::eventCancel(int eventId)
{
    // push the event ID onto the cancelled vector
    eventMutex.lock();
    eventCancelled.push_back(eventId);
    eventMutex.unlock();
}

void EventScheduler::planEvent(void* arg)
{
    EventScheduler* es = (EventScheduler*) arg;

    es->eventMutex.lock();
    Event e = es->eventQueue.top();
    es->eventQueue.pop();
    es->eventMutex.unlock();

    if (select(0, NULL, NULL, NULL, &e.trigTime) < 0) {
        cout << "error with select: " << strerror(errno) << endl;
        exit(1);
    }

    es->eventMutex.lock();
    for (std::vector<int>::iterator i = es->eventCancelled.begin(); i != es->eventCancelled.end(); ++i) {
        if (e.id == *i) {
            es->eventMutex.unlock();
            es->eventCancelled.erase(i);
            return;
        }
    }
    es->eventMutex.unlock();

    (*(e.funk))(e.arg);
}