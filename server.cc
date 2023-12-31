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

double Server :: exponential(double mean) {

	double r = (double)rand()/RAND_MAX;
	double ex = -(1/mean) * log (r);
	return ex;
}

void Server :: initialize () {
    status_ = 0;

    itemArrived_ = 0;
    itemCompleted_ = 0;
	
    totalDelay_ = 0.0;
    totalServiceTime_ = 0.0;
	totalInterarrivalTime_ = 0.0;
	totalSystemTime_ = 0.0;

    totalQueJobTime_ = 0.0;
	totalServerJobTime_ = 0.0;
	totalSystemJobTime_ = 0.0;
	
    lastEventTime_ = 0;
	lastArrivalTime_ = 0;

	srand (time (0) + (100 * arrivalMean_));
    
    double t = exponential(arrivalMean_);
    a_.schedule(t); 
	
}

void Server :: createTraceFile () {
	trace_.open("JobAvgVar.csv", ios::app);
    
    if (!trace_) {
		cout << "cannot open the JobAvgVar file.\n";
	}
	else{
       trace_ << "(arrivalMean_ / departureMean_)" << "," << "item->arrivalTime_" << "," << "queue_->length()" << "," << "status_" << ",\n";
	}
}

void Server :: createTraceFile (string name) {
	trace_.open(name + ".csv", ios::app);

    if (!trace_) {
		cout << "cannot open the "+name+" file.\n";
	}
	else{
       trace_ << "(arrivalMean_ / departureMean_)" << "," << "item->arrivalTime_" << "," << "queue_->length()" << "," << "status_" << ",\n";
	}
}

Item* Server::jobArrival () {
    Item* item;
    double now = Scheduler::now();

	itemArrived_++;

	item = new Item();
    item->id_ = itemArrived_;
    item->arrivalTime_ = now;
	
    trace_ << (arrivalMean_ / departureMean_) << "," << item->arrivalTime_ << "," << queue_->length() << "," << status_ << ",\n";

	return item;
}

void Server :: arrivalHandler () 
{
    int flag;

    updateTimeAvgStat();
    Item* item = jobArrival();
    flag = updateStateArr();

    if (flag == 0) {
        double st = serve(item);

        updateJobStatVar(0, st);
        updateEventList(0, st);
    } 
    else {
        queue_->enque(item);

        itemInQueue_ = item;

        updateJobStatVar(1, -1);
        updateEventList(1, -1);
    }
	
}

void Server :: updateTimeAvgStat () 
{
    double interEventTime = Scheduler::now() - lastEventTime_;
    totalQueJobTime_ = interEventTime * queue_->length();
    totalServerJobTime_ = interEventTime * status_;
    lastEventTime_ = Scheduler::now();
}

int Server::updateStateArr () 
{
    if (status_ == 0) {
        status_ = 1;
        return 0;
    } 
    else return 1;
}

double Server::serve(Item* item)
{
    itemInService_ = item;
    double serviceTime = exponential (departureMean_);

	return serviceTime;
}

void Server::updateJobStatVar (int type, double value) 
{
    if (type == 0) {
        itemInService_->interarrivalTime_ = itemInService_->arrivalTime_ - lastArrivalTime_;
        itemInService_->delay_ = 0;
        itemInService_->serviceTime_ = value;

        lastArrivalTime_ = Scheduler::now();
    } 
    else if (type == 1) {
        itemInQueue_->interarrivalTime_ = itemInQueue_->arrivalTime_ - lastArrivalTime_;

        lastArrivalTime_ = Scheduler::now();
    } 
    else {
        itemInService_->delay_ = Scheduler::now();
        itemInService_->serviceTime_ = value;
    }

}

void Server::updateEventList (int type, double value) {
    if (type == 0) {
        if (itemArrived_ < 500) {
            double t = exponential(arrivalMean_);
            a_.schedule(t);
        }

        d_.schedule(value);
    } 
    else if (type == 1) {
        if (itemArrived_ >= 500) return;

        double t = exponential(arrivalMean_);
        a_.schedule(t);
    } 
    else {
        d_.schedule(value);
    }
}

void Server :: departureHandler () 
{
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
}

void Server::updateJobAvgStat (){
    totalDelay_ += itemInService_->delay_;
    totalServiceTime_ += itemInService_->serviceTime_;
    totalInterarrivalTime_ += itemInService_->interarrivalTime_;
}

void Server::jobDeparture () {
    itemCompleted_++;

    if (itemCompleted_ == 500) {
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

int Server::updateStateDep () 
{
    if (queue_->length() > 0) return 1;
    else {
        status_ = 0;
        return 0;
    }
}

Server::~Server()
{
    delete queue_;
}
