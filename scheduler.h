#ifndef scheduler_h
#define scheduler_h

#include "event.h"

class Scheduler
{
	private:
		void addEvent (Event *e);
		Event* removeEvent ();
		void updateClock (double t);	
    public:
		static Event* eventList_;
		Scheduler ()
        {
            eventList_=0;
            clock_=0;
        }
		void trigger ();
		void run ();
		static double now ();
		static Scheduler& instance ();		
		void schedule (Event *e);
		void cancel (Event *e);
		static double clock_;
		static Scheduler* instance_;
};

#endif    
