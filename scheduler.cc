#include "scheduler.h"
#include "event.h"

#include "iostream"

using namespace std;

Event* Scheduler :: eventList_;
double Scheduler :: clock_;
Scheduler* Scheduler::instance_;

Scheduler::Scheduler()
{
    eventList_ = 0;
}

double
Scheduler :: now () {
	return clock_;
}

Scheduler&
Scheduler :: instance () {
	return (*instance_);
}

void
Scheduler :: updateClock (double t) {
	// ------------------------------------Task-1 (start)--------------------------------------------------
    // update clock time
	clock_ = t;
	// ------------------------------------Task-1 (end)--------------------------------------------------
}

void
Scheduler :: initialize () {
    // ------------------------------------Task-2 (start)--------------------------------------------------
    // initialize the simulator
    // Hint: What's the value of clock in the beginning?
	clock_ = 0;
	// ------------------------------------Task-2 (end)--------------------------------------------------
}

void
Scheduler :: activate (Event* e) {
	addEvent (e);
}

void
Scheduler :: addEvent (Event *e) {

	Event *current;
	Event *previous;

	// add in an empty list
	if (!eventList_) {
		eventList_ = e;
		e->next_ = 0;

		return;
	}

	// add as a first element in a non-empty list
	//previous = eventList_;
	// ------------------------------------Task-3 (start)--------------------------------------------------
	// Add the event as a first element in a non-empty list
	// Hint: check for event expiry time
	// Don't forget to return after adding the event
	if (e->expire () < eventList_->expire()) {
		e->next_ = eventList_;
		eventList_ = e;

		return;
	}
	// ------------------------------------Task-3 (end)--------------------------------------------------


	// add as an intermediate element
	previous = eventList_;
	current = previous -> next_;
	// ------------------------------------Task-4 (start)--------------------------------------------------
	// Add the event as an intermediate element of the eventlist
	while (current != 0) {
		if (e->expire() < current->expire ()) {
			e->next_ = current;
			previous->next_=e;
			return;
		} else {
			current = current->next_;
			previous = previous->next_;
		}
	}
	// ------------------------------------Task-4 (end)--------------------------------------------------

	// add as the last element
	previous->next_ = e;
	e->next_ = 0;

	return;
}

void
Scheduler :: deactivate (Event* e) {
    removeEvent (e);
}


Event*
Scheduler :: removeEvent (Event* e) {
    Event *current;
	Event *previous;
	Event *temp;

	// add in an empty list
	if (!eventList_) {
		cout << "List is empty\n";

		return 0;
	}


	// ------------------------------------Task-3 (start)--------------------------------------------------
	// remove the first event from a non-empty list
	// Hint: check for event type
	// Don't forget to return after adding the event
	if (e-> eventType () == eventList_-> eventType ()) {
        temp = eventList_;
		eventList_= eventList_ -> next_;

		return temp;
	}
	// ------------------------------------Task-3 (end)--------------------------------------------------


	// remove an intermediate event or the last event
	previous = eventList_;
	current = previous -> next_;
	// ------------------------------------Task-4 (start)--------------------------------------------------
	// remove an intermediate event or the last event
	while (current != 0) {
		if (e -> eventType () == current -> eventType ()) {
			temp = current;
			previous->next_= current -> next_;

			return temp;
		} else {
			previous = current;
			current = current->next_;
		}
	}
	// ------------------------------------Task-4 (end)--------------------------------------------------
	return 0;
}


// ------------------------------------Task-5 (start)--------------------------------------------------
// Complete the run() function
// Follow the flow-chart provided in theory class
void
Scheduler :: run () {
	Event * temp;

	while (eventList_ != 0) {

		temp = removeEvent ();

		trigger(temp);

		/*if (temp->eventType() == 1)
           cout << Scheduler::now () <<"  in run for arrival event" << endl;
        else
            cout << Scheduler::now () << "  in run for departure event" << endl;*/
	}
}
// ------------------------------------Task-5 (end)--------------------------------------------------

Event*
Scheduler :: removeEvent () {
	Event* temp;

	temp = eventList_;
	eventList_ = eventList_->next_;

	return temp;
}

void
Scheduler :: trigger (Event* e) {
	updateClock (e->expire ());

	e->handle ();
}


Scheduler::~Scheduler()
{
    //dtor
}
