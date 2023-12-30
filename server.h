#ifndef server_h
#define server_h

#include "event.h"
#include "queue.h"
//#include "scheduler.h"

#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

class Server
{
    private:
		Queue* queue_;
		ArrivalEvent a_;
		DepartureEvent d_;

		int status_;
		int itemArrived_;
		Item* itemInService_;		
		ofstream trace_;

		double arrivalMean_;
		double departureMean_;
		double exponential (double mean);
        
	public:
		Server () : a_(this), d_(this)
		{
			queue_ = new Queue ();
		}
		void initialize ()
		{
			status_ = 0;
			itemArrived_ = 0;
			
			double t = exponential (arrivalMean_);
			
			a_.activate (t);
			
		}
		void arrivalHandler (){
			Item* temp;

			itemArrived_++;
			temp = (Item*) malloc (sizeof(Item));
			temp->id_ = itemArrived_;

			if (status () == 0) {
				status() = 1;
				status() = 1;
				itemInService_ = temp;

				double t = exponential (departureMean_);
				d_.activate (t);
			} 
			else {
				queue_->enque(temp);
			}

			if (temp->id_ < 100) {
				double t = exponential (arrivalMean_);
				a_.activate (t);
			}
		}
		void departureHandler (){
			if (queue_->length() > 0) {
				itemInService_ = queue_->deque ();

				double t = exponential (departureMean_);
				d_.activate (t);
			}
			else {
				status () = 0;
				itemInService_ = 0;
			}
		}

		void createTraceFile (){
			trace_.open ("trace.out", ios::out);
			if (!trace_) {
				cout << "cannot open the trace file.\n";
			}
			trace_<< "trace file for the simulation" << endl;
			trace_ << "format of the file" << endl;
			trace_ << "<event> <time> <item id> <server status> <queue size>" << endl << endl;
		}

		inline int& status () { return (status_); }
		inline int& itemArrived () { return (itemArrived_); }
		inline double& arrivalMean () { return (arrivalMean_); }
		inline double& departureMean () { return (departureMean_); }
};
#endif   