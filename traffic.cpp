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
            cout << counter << endl;
        }
    }

    cout << counter << " Cars \n";
}

// void Traffic::trafficLightsLoop() {
//     // chrono::seconds startTime;
//
//     auto start = chrono::steady_clock::now();
//     // auto startTime = chrono::duration_cast<chrono::seconds>(start.time_since_epoch());
//     chrono::seconds startTime = static_cast<chrono::seconds>(0);
//
//     northSouthLight.currentLight = greenLight;
//     cout << "northSouthLight is green, eastWestLight is red. \n";
//
//     while (running) {
//         // chrono::seconds currentTime;
//         auto now = chrono::steady_clock::now();
//
//         auto currentTime = chrono::duration_cast<chrono::seconds>(now - start);
//
//         // cout<<currentTime<<endl;
//
//         if (currentTime >= startTime + northSouthLight.greenLightTime && northSouthLight.currentLight == greenLight) {
//             northSouthLight.currentLight = redLight;
//             eastWestLight.currentLight = greenLight;
//
//             cout << "northSouthLight is red, eastWestLight is green. \n";
//
//             startTime = currentTime;
//         }else if (currentTime >= startTime + eastWestLight.greenLightTime && eastWestLight.currentLight == greenLight) {
//             eastWestLight.currentLight = redLight;
//             northSouthLight.currentLight = greenLight;
//
//             cout << "northSouthLight is green, eastWestLight is red. \n";
//
//
//             startTime = currentTime;
//         }
//     }
// }


void Traffic::trafficLightsLoop() {
    northSouthLight.currentLight = greenLight;
    cout << "northSouthLight is green, eastWestLight is red. \n";

    while (running) {
        auto timerDuration = (northSouthLight.currentLight == greenLight)
                                 ? northSouthLight.greenLightTime
                                 : eastWestLight.greenLightTime;

        auto now = chrono::steady_clock::now();

        while (running) {
            auto currentTime = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - now);

            if (currentTime >= timerDuration) {
                break;
            } else {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }

        if (northSouthLight.currentLight == greenLight) {
            northSouthLight.currentLight = redLight;
            eastWestLight.currentLight = greenLight;

            cout << "northSouthLight is red, eastWestLight is green. \n";
        } else if (eastWestLight.currentLight == greenLight) {
            eastWestLight.currentLight = redLight;
            northSouthLight.currentLight = greenLight;

            cout << "northSouthLight is green, eastWestLight is red. \n";
        }
    }
}

void createIntersection(Traffic &intersection) {
    int userInput = 0;

    cout << "North bound";
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

    cout << "East bound";
    cin >> userInput;
    intersection.eastBoundLanes = userInput;
    // cout<<"East left turn lanes";
    // cin >> userInput;
    // intersection.eastLeftTurnLanes = userInput;
    // cout<<"East right turn lanes";
    // cin >> userInput;
    // intersection.eastRightTurnLanes = userInput;


    cout << "South bound";
    cin >> userInput;
    intersection.southBoundLanes = userInput;
    // cout<<"South left turn lanes";
    // cin >> userInput;
    // intersection.southLeftTurnLanes = userInput;
    // cout<<"South right turn lanes";
    // cin >> userInput;
    // intersection.southRightTurnLanes = userInput;

    cout << "West bound";
    cin >> userInput;
    intersection.westBoundLanes = userInput;
    // cout<<"West left turn lanes";
    // cin >> userInput;
    // intersection.westLeftTurnLanes = userInput;
    // cout<<"West right turn lanes";
    // cin >> userInput;
    // intersection.westRightTurnLanes = userInput;

    cout << "North South Light timer: ";
    cin >> userInput;
    intersection.northSouthLight.greenLightTime = static_cast<chrono::seconds>(userInput);

    cout << "East West Light timer: ";
    cin >> userInput;
    intersection.eastWestLight.greenLightTime = static_cast<chrono::seconds>(userInput);
}

void printIntersection(const Traffic &intersection) {
    cout << "North bound " << intersection.northBoundLanes << "\n";
    cout << "North left turn lanes " << intersection.northLeftTurnLanes << "\n";
    cout << "North right turn lanes " << intersection.northRightTurnLanes << "\n";
    for (const auto &vehicle: northHeaded) {
        vehicle->print();
    }

    cout << "East bound " << intersection.eastBoundLanes << "\n";
    cout << "East left turn lanes " << intersection.eastLeftTurnLanes << "\n";
    cout << "East right turn lanes " << intersection.eastRightTurnLanes << "\n";
    for (const auto &vehicle: eastHeaded) {
        vehicle->print();
    }

    cout << "South bound " << intersection.southBoundLanes << "\n";
    cout << "South left turn lanes " << intersection.southLeftTurnLanes << "\n";
    cout << "South right turn lanes " << intersection.southRightTurnLanes << "\n";
    for (const auto &vehicle: southHeaded) {
        vehicle->print();
    }

    cout << "West bound " << intersection.westBoundLanes << "\n";
    cout << "West left turn lanes " << intersection.westLeftTurnLanes << "\n";
    cout << "West right turn lanes " << intersection.westRightTurnLanes << "\n";
    for (const auto &vehicle: westHeaded) {
        vehicle->print();
    }
}
