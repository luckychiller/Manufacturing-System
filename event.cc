#include "event.h"
#include "server.h"
#include "scheduler.h"

void Event :: schedule (double t) {
	Scheduler &s = Scheduler :: instance ();
   	stime_ = Scheduler :: now ();
	rtime_ = stime_ + t;

	s.activate (this);
}

void Event :: cancel () {
    Scheduler &s = Scheduler :: instance ();

    s.deactivate (this);
}

void ArrivalEvent :: handle () {
	server_-> arrivalHandler ();
} 

void DepartureEvent :: handle () {
	server_-> departureHandler ();
}