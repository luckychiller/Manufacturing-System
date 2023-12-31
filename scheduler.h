#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "event.h"

class Scheduler
{
    public:
        Scheduler();

        void run ();
		void initialize ();

		static double now ();
		static Scheduler& instance ();

		void activate (Event *e);
		void deactivate (Event *e);
		void cancel ();


        virtual ~Scheduler();

    protected:


    private:
        void addEvent (Event *e);
        Event* removeEvent (Event* e);
		Event* removeEvent ();

		void updateClock (double t);
		void trigger (Event* e); // trigger an event by removing it from the event list and calling the event handler

    public: //need to check private works or not
        static double clock_;
		static Event* eventList_;
		static Scheduler* instance_;


};

#endif // SCHEDULER_H
