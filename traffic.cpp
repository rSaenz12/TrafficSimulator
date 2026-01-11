//
// Created by rusty on 1/10/2026.
//

#include <iostream>
// #include <bits/this_thread_sleep.h>
#include <chrono>
#include <thread>

#include "traffic.h"

using namespace std;

void Traffic::trafficLoop() {
    int counter = 0;
    while (running) {
        populate(speedLimit);
        this_thread::sleep_for(chrono::milliseconds(500));
        counter++;

        if (counter % 15 == 0) {
            cout<<counter<<endl;
        }
    }

    cout<<counter<<" Cars \n";
}

void createIntersection(Traffic &intersection) {
    int userInput=0;

    cout<<"North bound";
    cin >> userInput;
    intersection.northBoundLanes = userInput;
    //
    // cout<<"North left turn lanes";
    // cin >> userInput;
    // intersection.northLeftTurnLanes = userInput;
    //
    // cout<<"North right turn lanes";
    // cin >> userInput;
    // intersection.northRightTurnLanes = userInput;

    cout<<"East bound";
    cin >> userInput;
    intersection.eastBoundLanes = userInput;
    // cout<<"East left turn lanes";
    // cin >> userInput;
    // intersection.eastLeftTurnLanes = userInput;
    // cout<<"East right turn lanes";
    // cin >> userInput;
    // intersection.eastRightTurnLanes = userInput;

    cout<<"South bound";
    cin >> userInput;
    intersection.southBoundLanes = userInput;
    // cout<<"South left turn lanes";
    // cin >> userInput;
    // intersection.southLeftTurnLanes = userInput;
    // cout<<"South right turn lanes";
    // cin >> userInput;
    // intersection.southRightTurnLanes = userInput;

    cout<<"West bound";
    cin >> userInput;
    intersection.westBoundLanes = userInput;
    // cout<<"West left turn lanes";
    // cin >> userInput;
    // intersection.westLeftTurnLanes = userInput;
    // cout<<"West right turn lanes";
    // cin >> userInput;
    // intersection.westRightTurnLanes = userInput;
}

void printIntersection(const Traffic &intersection) {
    cout<<"North bound " << intersection.northBoundLanes << "\n";
    cout<<"North left turn lanes " << intersection.northLeftTurnLanes << "\n";
    cout<<"North right turn lanes " << intersection.northRightTurnLanes << "\n";
    for (const auto &vehicle : northHeaded) {
        vehicle->print();
    }

    cout<<"East bound " << intersection.eastBoundLanes << "\n";
    cout<<"East left turn lanes " << intersection.eastLeftTurnLanes << "\n";
    cout<<"East right turn lanes " << intersection.eastRightTurnLanes << "\n";
    for (const auto &vehicle : eastHeaded) {
        vehicle->print();
    }

    cout<<"South bound " << intersection.southBoundLanes << "\n";
    cout<<"South left turn lanes " << intersection.southLeftTurnLanes << "\n";
    cout<<"South right turn lanes " << intersection.southRightTurnLanes << "\n";
    for (const auto &vehicle : southHeaded) {
        vehicle->print();
    }

    cout<<"West bound " << intersection.westBoundLanes << "\n";
    cout<<"West left turn lanes " << intersection.westLeftTurnLanes << "\n";
    cout<<"West right turn lanes " << intersection.westRightTurnLanes << "\n";
    for (const auto &vehicle : westHeaded) {
        vehicle->print();
    }
}


