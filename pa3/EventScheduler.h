/****************************************************
 * Author: Alex Brisnehan                           *
 * Date: 10/27/2014                                 *
 * File: EventScheduler.h                           *
 * Use: The header file for the EventScheduler class*
 ****************************************************/

#ifndef _EVENTSCHEDULER_
#define _EVENTSCHEDULER_

#include <list>
#include <string.h>

using namespace std;

class EventScheduler {
    public:
	EventScheduler(size_t maxEvents);
	~EventScheduer();
	int eventSchedule(void evFunction(void *), void * arg, int timeout);
	void eventCancel(int eventId);
    private:
	int eventId;
}

#endif
