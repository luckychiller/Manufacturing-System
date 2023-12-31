#ifndef EVENT_H
#define EVENT_H


class Server;

class Event
{
    public:
        Event (Server* s) : server_ (s) {
 			stime_ = 0.0;
			rtime_ = 0.0;
		}

   		inline int& eventType () { return (eventType_); }
		inline double& expire () { return (rtime_); }
		inline double& start () { return (stime_); }

		void schedule (double t);
		void cancel ();
		virtual void handle () = 0;

		Event* next_;

        //virtual ~Event();

    protected:
        Server* server_;

    private:
        int eventType_;	// represented by event id
		double stime_;
		double rtime_;
};

class ArrivalEvent : public Event
{
	public:
 		ArrivalEvent (Server* s) : Event(s) {
            eventType() = 1;
 		}
		void handle ();
};

class DepartureEvent : public Event
{
	public:
		DepartureEvent (Server* s) : Event(s) {
            eventType () = 2;
		}
		void handle ();
};


#endif // EVENT_H
