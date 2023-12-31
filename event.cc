#include "event.h"
#include "server.h"
#include "scheduler.h"

void
Event :: schedule (double t) {
	Scheduler &s = Scheduler :: instance ();
   	stime_ = Scheduler :: now ();
	rtime_ = stime_ + t;

	s.activate (this);
}

void
Event :: cancel () {
    Scheduler &s = Scheduler :: instance ();

    s.deactivate (this);
}

// ------------------------------------Task-11 (start)--------------------------------------------------
// handle an arrival event
void
ArrivalEvent :: handle () {
	server_-> arrivalHandler ();
}
// ------------------------------------Task-11 (end)--------------------------------------------------

// ------------------------------------Task-12 (start)--------------------------------------------------
// handle a departure event
void
DepartureEvent :: handle () {
	server_-> departureHandler ();
}
// ------------------------------------Task-12 (end)--------------------------------------------------
