#ifndef SERVER_H
#define SERVER_H

#include "event.h"
#include "queue.h"

#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

class Server
{
    public:
        Server();

        void initialize ();
        void arrivalHandler ();
		void departureHandler ();

		void createTraceFile ();
		void createTraceFile (string name);


		inline int& status () { return (status_); }
		inline int& itemArrived () { return (itemArrived_); }
		inline double& arrivalMean () { return (arrivalMean_); }
		inline double& departureMean () { return (departureMean_); }

		// events of the system
        ArrivalEvent a_;
        DepartureEvent d_;

        virtual ~Server();

    protected:

    private:
        //update system state and even list
        int updateStateArr ();
        int updateStateDep ();
        //void updateStatus (int evnt);
        double serve (Item* item);
        void updateDelay (int evnt, double st, Item* item);
        void updateEventList (int type, double value);

        //arrival and departure of jobs
        Item* jobArrival ();
        void jobDeparture ();

        // update statistical variables
        void updateJobStatVar (int type, double value);
        void updateJobStatVar2 (Item* item); // function to correct the above
        void updateTimeAvgStat ();
        void updateJobAvgStat ();



        // State variables
        int status_; // server status
        Queue* queue_; // length_ variable in queue



		// job/customer in service
		Item* itemInService_;
		Item* itemInQueue_;
		int itemArrived_;
		int itemCompleted_;

		// input variables
		double arrivalMean_;
		double departureMean_;
		// random number generator
		double exponential (double mean);

		//job average variables
		double totalDelay_;
		double totalServiceTime_;
		double totalInterarrivalTime_;
		double totalSystemTime_;

		//time average variables
		double totalQueJobTime_;
		double totalServerJobTime_;
		double totalSystemJobTime_;

		double lastEventTime_; //time instance of the occurrence of the last event
		double lastArrivalTime_; // time instance of the last arrival

		//conditional variables
		const double MAX_DELAY = 30;
		const long MAX_DELAY_NO = 100;
		long maxDelayCount_;

		//trace file
		ofstream trace_;
};

#endif // SERVER_H
