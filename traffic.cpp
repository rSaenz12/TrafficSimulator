//
// Created by rusty on 1/10/2026.
//

#include <iostream>
// #include <bits/this_thread_sleep.h>
#include <chrono>
#include <thread>

#include "traffic.h"

using namespace std;

void delay(int delayTimer, const std::atomic<bool> &emergencyExit) {
    if (delayTimer>500) {
        cout<<"Timer exceeds 500 seconds \n";
        return;
    }

    const std::chrono::seconds timerDuration{delayTimer};

    const auto now = chrono::steady_clock::now();

    while (emergencyExit) {
        auto currentTime = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - now);

        if (currentTime >= timerDuration) {
            break;
        }else {
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
}

void Traffic::trafficLoop() {
    int counter = 0;
    while (running) {
        populate(speedLimit);

        delay(1, running);

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

        delay(static_cast<int>(timerDuration.count()), running);


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

int Traffic::checkActiveLane() {
    for (TrafficLight *light : lightsVector) {
        if (light->currentLight == greenLight) {
            return light->headingID;
        }
    }

    //error handled, shouldnt crash but no cars will leaving seeing green
    return -1;
}

//change all the headings vectors to deque so that you can use deque.pop_front(), its faster and better than vector.erase(vector.begin()).
// void Traffic::popVehicles(vector<unique_ptr<Vehicles>> &headingVector, int numberOfLanes, string direction) {
void Traffic::popVehicles(deque<unique_ptr<Vehicles>> &headingVector, int numberOfLanes, string direction) {
    for (int i=0; i<numberOfLanes; i++) {
        if (!headingVector.empty()) {
            // headingVector.pop_back();
            // headingVector.erase(headingVector.begin());
            headingVector.pop_front();
            cout<<"popped "<<direction<<"\n";
        }else {
            break;
        }
    }
}

void Traffic::passVehiclesThroughIntersection() {

    while (running) {

        delay(1, running);

        int currentHeading = checkActiveLane();
        switch (currentHeading) {
            case northSouth: {
                //for loops added because if x lanes, x amount of cars at the front that go.
                // for (int i=0; i<northBoundLanes; i++) {
                //   if (!northHeaded.empty()){
                //     northHeaded.pop_back();
                //     cout<<"popped North \n";
                //   }else {
                //       break;
                //   }
                // }
                //
                // for (int i=0; i<southBoundLanes;i++){
                //     if (!southHeaded.empty()) {
                //         southHeaded.pop_back();
                //         cout<<"popped South \n";
                //     }else {
                //         break;
                //     }
                // }

                popVehicles(northHeaded, northBoundLanes, "North");
                popVehicles(southHeaded, southBoundLanes, "South");

                break;
            }
            case eastWest: {
                // for (int i=0; i<eastBoundLanes; i++){
                //     if (!eastHeaded.empty() ){
                //         eastHeaded.pop_back();
                //         cout<<"popped East \n";
                //     }else {
                //         break;
                //     }
                // }
                //
                // for (int i=0; i<westBoundLanes; i++){
                //     if (!westHeaded.empty()){
                //         westHeaded.pop_back();
                //         cout<<"popped West \n";
                //     }else {
                //         break;
                //     }
                // }

                popVehicles(eastHeaded, eastBoundLanes, "East");
                popVehicles(westHeaded, westBoundLanes, "West");

                break;
            }
        }

    }

}

void Traffic::addLights() {
    lightsVector.push_back(&northSouthLight);
    lightsVector.push_back(&northSouthTurnLight);

    lightsVector.push_back(&eastWestLight);
    lightsVector.push_back(&eastWestTurnLight);
}

void checkUserInput(int &userInput) {
    if (userInput>=0) {
    }else {
        userInput = 0;
    }
}

void createIntersection(Traffic &intersection) {
    int userInput = 0;

    cout << "North bound";
    cin >> userInput;
    checkUserInput(userInput);
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
    checkUserInput(userInput);
    intersection.eastBoundLanes = userInput;
    // cout<<"East left turn lanes";
    // cin >> userInput;
    // intersection.eastLeftTurnLanes = userInput;
    // cout<<"East right turn lanes";
    // cin >> userInput;
    // intersection.eastRightTurnLanes = userInput;


    cout << "South bound";
    cin >> userInput;
    checkUserInput(userInput);
    intersection.southBoundLanes = userInput;
    // cout<<"South left turn lanes";
    // cin >> userInput;
    // intersection.southLeftTurnLanes = userInput;
    // cout<<"South right turn lanes";
    // cin >> userInput;
    // intersection.southRightTurnLanes = userInput;

    cout << "West bound";
    cin >> userInput;
    checkUserInput(userInput);
    intersection.westBoundLanes = userInput;
    // cout<<"West left turn lanes";
    // cin >> userInput;
    // intersection.westLeftTurnLanes = userInput;
    // cout<<"West right turn lanes";
    // cin >> userInput;
    // intersection.westRightTurnLanes = userInput;

    cout << "North South Light timer: ";
    cin >> userInput;
    checkUserInput(userInput);
    intersection.northSouthLight.greenLightTime = static_cast<chrono::seconds>(userInput);

    cout << "East West Light timer: ";
    cin >> userInput;
    checkUserInput(userInput);
    intersection.eastWestLight.greenLightTime = static_cast<chrono::seconds>(userInput);

    intersection.addLights();
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
