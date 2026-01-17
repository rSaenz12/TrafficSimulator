#include <iostream>
#include <vector>

// #include <bits/this_thread_sleep.h>

#include <chrono>
#include <thread>

#include <conio.h>

#include "traffic.h"
#include "vehicles.h"


using namespace std;

void delay(int delayTimer);

int main() {
    srand(time(nullptr));

    thread trafficThread;
    thread trafficLights;
    thread runningTraffic;

    Traffic intersection(45);
    createIntersection(intersection);

    int userInput = 0;

    cout<<"ENTER Number >= 1 to start \n";
    while (userInput < 1) {
        cin>>userInput;
    }

    if (userInput>0) {
        intersection.running = true;
        trafficThread = thread(&Traffic::trafficLoop, &intersection);
        trafficLights = thread(&Traffic::trafficLightsLoop, &intersection);
        runningTraffic = thread(&Traffic::passVehiclesThroughIntersection, &intersection);
    }

    userInput = 0;

    cout<<"ENTER Number >= 1 to stop \n";
    while (userInput < 1) {
        cin>>userInput;
    }

    if (userInput > 0) {
        intersection.running = false;

        trafficThread.join();
        trafficLights.join();
        runningTraffic.join();
    }


    printIntersection(intersection);
    return 0;
}

