#include <iostream>
#include <fstream>
using namespace std;

#include "scheduler.h"
#include "server.h"

int main()
{
    Scheduler *bodyScheduler = new Scheduler(); // to schedule the first arrival
    Server *bodyShop = new Server();
    bodyShop->createTraceFile();
    bodyShop->arrivalMean() = 5.0;
    bodyShop->departureMean() = 3.0;
    bodyShop->initialize();
    bodyScheduler->run();
    return 0;
}
