#include <iostream>
#include <vector>

// #include <bits/this_thread_sleep.h>

#include <chrono>
#include <thread>

#include <conio.h>

#include "traffic.h"
#include "vehicles.h"


using namespace std;
int main() {
    srand(time(nullptr));

    thread trafficThread;

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
    }

    userInput = 0;

    cout<<"ENTER Number >= 1 to stop \n";
    while (userInput < 1) {
        cin>>userInput;
    }

    if (userInput > 0) {
        intersection.running = false;
        trafficThread.join();
    }

    printIntersection(intersection);
    return 0;
}