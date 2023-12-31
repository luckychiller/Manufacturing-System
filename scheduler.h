#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "event.h"

class Scheduler
{
    public:
        Scheduler();
		
		static double clock_;
		static Event* eventList_;
		static Scheduler* instance_;

        void run ();
		void initialize ();

		static double now ();
		static Scheduler& instance ();

		void activate (Event *e);
		void deactivate (Event *e);
		void cancel ();


        virtual ~Scheduler();

    private:
        void addEvent (Event *e);
        Event* removeEvent (Event* e);
		Event* removeEvent ();

		void updateClock (double t);
		void trigger (Event* e); 

};

#endif // SCHEDULER_H
