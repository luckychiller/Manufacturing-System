#include "server.h"
#include "scheduler.h"

#include <malloc.h>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;


Server::Server() : a_(this), d_(this)
{
    queue_ = new Queue ();
}


double
Server :: exponential(double mean) {

	double r = (double)rand()/RAND_MAX;

	double ex = -(1/mean) * log (r);

	return ex;
}


// initialize the server
void
Server :: initialize () {
    // ------------------------------------Task-1 (start)--------------------------------------------------
    // initialize the state variables
    status_ = 0;

    itemArrived_ = 0;
    itemCompleted_ = 0;
	// ------------------------------------Task-1 (end)----------------------------------------------------

	// ------------------------------------Task-2 (start)--------------------------------------------------
    // initialize the statistical variables
    //job average variables
    totalDelay_ = 0.0;
    totalServiceTime_ = 0.0;
	totalInterarrivalTime_ = 0.0;
	totalSystemTime_ = 0.0;

    //time average variables
    totalQueJobTime_ = 0.0;
	totalServerJobTime_ = 0.0;
	totalSystemJobTime_ = 0.0;
	// ------------------------------------Task-2 (end)----------------------------------------------------

	//inter-event time of the first event is calculated starting from 0
	lastEventTime_ = 0;
	lastArrivalTime_ = 0;


	srand (time (0) + (100 * arrivalMean_)); // set the seed of the random number generator to current timen12

    // ------------------------------------Task-3 (start)--------------------------------------------------
    // add the event feasible at the beginning
    double t = exponential(arrivalMean_);
    a_.schedule(t);  //fix
	// ------------------------------------Task-3 (end)--------------------------------------------------
}

// Create the trace file
void
Server :: createTraceFile () {
	trace_.open("JobAvgVar.csv", ios::app);

    if (!trace_) {
		cout << "cannot open the JobAvgVar file.\n";
	}
	else
    {
       trace_ << "(arrivalMean_ / departureMean_)" << "," << "item->arrivalTime_" << "," << "queue_->length()" << "," << "status_" << ",\n";
	}
}

void
Server :: createTraceFile (string name) {
	trace_.open(name + ".csv", ios::app);

    if (!trace_) {
		cout << "cannot open the "+name+" file.\n";
	}
	else
    {
       trace_ << "(arrivalMean_ / departureMean_)" << "," << "item->arrivalTime_" << "," << "queue_->length()" << "," << "status_" << ",\n";
	}
}


// create a new job
// Remember to write to the trace file following the trace file format
Item*
Server::jobArrival () {
    Item* item;
    double now = Scheduler::now();

	itemArrived_++;

	// ------------------------------------Task-4 (start)--------------------------------------------------
    // create a new job
    item = new Item();
    item->id_ = itemArrived_;
    item->arrivalTime_ = now;
	// ------------------------------------Task-4 (end)--------------------------------------------------

    trace_ << (arrivalMean_ / departureMean_) << "," << item->arrivalTime_ << "," << queue_->length() << "," << status_ << ",\n";

	return item;
}





void
Server :: arrivalHandler () {
    // flag to keep track of the idle or busy at arrival
    int flag;


    // ------------------------------------Task-5 (start)--------------------------------------------------
    // call the necessary functions associated with arrival of a job
    updateTimeAvgStat();
    Item* item = jobArrival();
    flag = updateStateArr();

    if (flag == 0) {
        double st = serve(item);

        updateJobStatVar(0, st);
        updateEventList(0, st);
    } else {
        queue_->enque(item);

        itemInQueue_ = item;

        updateJobStatVar(1, -1);
        updateEventList(1, -1);
    }
	// ------------------------------------Task-5 (end)--------------------------------------------------
}


void
Server :: updateTimeAvgStat () {
    // ------------------------------------Task-6 add the event feasible at the beginning

    // update the time average statistical variables
    double interEventTime = Scheduler::now() - lastEventTime_;
    totalQueJobTime_ = interEventTime * queue_->length();
    totalServerJobTime_ = interEventTime * status_;
    lastEventTime_ = Scheduler::now();
	// ------------------------------------Task-6 (end)--------------------------------------------------
}



//Function to update the system state after the arrival event
int
Server::updateStateArr () {

    // ------------------------------------Task-7 (start)--------------------------------------------------
    // update the system state after arrival of an  event
    if (status_ == 0) {
        status_ = 1;
        return 0;
    } else return 1;
	// ------------------------------------Task-7 (end)--------------------------------------------------
}


double
Server::serve(Item* item){
    //put the item in service
    itemInService_ = item;

    // service time of the job
    double serviceTime = exponential (departureMean_);

	return serviceTime;
}

void Server::updateJobStatVar (int type, double value) {
    // ------------------------------------Task-8 (start)--------------------------------------------------
    // update the job average variable of each job
    if (type == 0) {
        itemInService_->interarrivalTime_ = itemInService_->arrivalTime_ - lastArrivalTime_;
        itemInService_->delay_ = 0;
        itemInService_->serviceTime_ = value;

        lastArrivalTime_ = Scheduler::now();
    } else if (type == 1) {
        itemInQueue_->interarrivalTime_ = itemInQueue_->arrivalTime_ - lastArrivalTime_;

        lastArrivalTime_ = Scheduler::now();
    } else {
        itemInService_->delay_ = Scheduler::now();
        itemInService_->serviceTime_ = value;
    }
	// ------------------------------------Task-8 (end)--------------------------------------------------
}

void
Server::updateEventList (int type, double value) {
    // ------------------------------------Task-9 (start)--------------------------------------------------
    // update the event list after arrival or departure
    if (type == 0) {
        if (itemArrived_ < 1000) {
            double t = exponential(arrivalMean_);
            a_.schedule(t);
        }

        d_.schedule(value);
    } else if (type == 1) {
        if (itemArrived_ >= 1000) return;

        double t = exponential(arrivalMean_);
        a_.schedule(t);
    } else {
        d_.schedule(value);
    }
	// ------------------------------------Task-9 (end)--------------------------------------------------
}



void
Server :: departureHandler () {
    // ------------------------------------Task-10 (start)--------------------------------------------------
    // call the necessary functions after departure of a job
    updateTimeAvgStat();
    updateJobAvgStat();
    jobDeparture();

    int flag = updateStateDep();

    if (flag) {
        Item* item = queue_->dque();
        double st = serve(item);
        updateJobStatVar(2, st);
        updateEventList(2, st);
    }
	// ------------------------------------Task-10 (end)--------------------------------------------------
}



void
Server::updateJobAvgStat (){
    // ------------------------------------Task-11 (start)--------------------------------------------------
    // update the job average statistical variables
    totalDelay_ += itemInService_->delay_;
    totalServiceTime_ += itemInService_->serviceTime_;
    totalInterarrivalTime_ += itemInService_->interarrivalTime_;
	// ------------------------------------Task-11 (end)--------------------------------------------------
}

void
Server::jobDeparture () {
    itemCompleted_++;

    if (itemCompleted_ == 1000) {
        trace_.close();

        trace_.open ("TimeAvgVar.csv", ios::app);

        if (!trace_) {
            cout << "cannot open the TimeAvgVar file.\n";
        }

        double avgQueDelay_ = totalDelay_ / itemCompleted_;
        double avgSysDelay_ = (totalServiceTime_ + totalDelay_) / itemCompleted_;
        double rho_ = arrivalMean_ / departureMean_;

        trace_ << rho_ << "," << avgQueDelay_ << "," << avgSysDelay_ << ",\n";

        trace_.close();
    }

    free(itemInService_);
}

//Function to update the system state after the departure event
int
Server::updateStateDep () {
    // ------------------------------------Task-12 (start)--------------------------------------------------
    // update the system state after the departure of a job
    if (queue_->length() > 0) return 1;
    else {
        status_ = 0;
        return 0;
    }
	// ------------------------------------Task-12 (end)--------------------------------------------------
}


Server::~Server()
{
    //dtor
    delete queue_;
}
