//
// Created by rusty on 1/10/2026.
//

#include <iostream>

#include <thread>

#include "traffic.h"


using namespace std;

chrono::seconds greenLightTimeRemaining(0);

void delay(int delayTimer, const std::atomic<bool> &emergencyExit, chrono::seconds *timeRemaining) {
    if (delayTimer > 50000) {
        cout << "Timer exceeds 50000  \n";
        delayTimer = 500;
    }

    delayTimer = delayTimer * 100;

    const std::chrono::milliseconds timerDuration{delayTimer};

    const auto now = chrono::steady_clock::now();

    while (emergencyExit) {
        auto currentTime = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - now);


        if (currentTime >= timerDuration) {
            break;
        } else {
            if (timeRemaining != nullptr) {
                auto remaining = timerDuration - currentTime;
                *timeRemaining = chrono::duration_cast<chrono::seconds>(remaining);
            }

            this_thread::sleep_for(chrono::milliseconds(30));
        }
    }
}

void Traffic::trafficLoop() {
    while (running) {
        populate(speedLimit);

        //90 chosen because 85 is highest speed, speedMultiplier max is 5
        //90 = 85 + 5, this stops from the int rounding to 0 and having a 0 second delay
        delay((90 - speedLimit) / speedMultiplier, running, nullptr);
    }
}

void Traffic::trafficLightsLoop() {

    //start on green initially
    northSouthLight.currentLight = greenLight;

    while (running) {
        auto timerDuration = (northSouthLight.currentLight == greenLight)
                                 ? northSouthLight.greenLightTime
                                 : eastWestLight.greenLightTime;

        //multiplying the timerDuration by 10 turns it to seconds in the delay function
        delay(timerDuration * 10 / speedMultiplier, running, &greenLightTimeRemaining);

        //flips green to red, red to green
        if (northSouthLight.currentLight == greenLight) {
            northSouthLight.currentLight = redLight;
            eastWestLight.currentLight = greenLight;
        } else if (eastWestLight.currentLight == greenLight) {
            eastWestLight.currentLight = redLight;
            northSouthLight.currentLight = greenLight;
        }
    }
}

uint8_t Traffic::checkActiveLane() {
    for (TrafficLight *light: lightsVector) {
        if (light->currentLight == greenLight) {
            return light->headingID;
        }
    }

    //error handled, shouldnt crash but no cars will leaving seeing green
    return 100;
}

void Traffic::popVehicles(deque<unique_ptr<Vehicles> > &headingVector, uint8_t numberOfLanes, string direction) {
    for (uint8_t i = 0; i < numberOfLanes; i++) {
        if (!headingVector.empty()) {
            headingVector.pop_front();
        } else {
            break;
        }
    }
}

void Traffic::passVehiclesThroughIntersection() {
    while (running) {
        delay(1, running, nullptr);

        const uint8_t currentHeading = checkActiveLane();
        switch (currentHeading) {
            case northSouth: {
                popVehicles(northHeaded, northBoundLanes, "North");
                popVehicles(southHeaded, southBoundLanes, "South");

                break;
            }
            case eastWest: {
                popVehicles(eastHeaded, eastBoundLanes, "East");
                popVehicles(westHeaded, westBoundLanes, "West");

                break;
            }
        }
    }
}

void Traffic::addLights() {
    lightsVector.push_back(&northSouthLight);

    lightsVector.push_back(&eastWestLight);
}

uint8_t checkUserInput(int userInput) {
    if (userInput >= 3) {
        userInput = 3;
    } else if (userInput < 0) {
        userInput = 0;
    }

    return static_cast<uint8_t>(userInput);
}

void createIntersection(Traffic &intersection, const uint8_t northSouthLanes, const uint8_t eastWestLanes,
                        const uint8_t speed, const uint8_t northSouthLightTimer, const uint8_t eastWestLightTimer) {
    intersection.northBoundLanes = checkUserInput(northSouthLanes);

    intersection.southBoundLanes = checkUserInput(northSouthLanes);

    intersection.eastBoundLanes = checkUserInput(eastWestLanes);

    intersection.westBoundLanes = checkUserInput(eastWestLanes);

    intersection.northSouthLight.greenLightTime = northSouthLightTimer;

    intersection.eastWestLight.greenLightTime = eastWestLightTimer;

    intersection.addLights();

    intersection.speedLimit = speed;
}


void printIntersection(const Traffic &intersection) {
    cout << "North bound " << intersection.northBoundLanes << "\n";
    for (const auto &vehicle: northHeaded) {
        vehicle->print();
    }

    cout << "East bound " << intersection.eastBoundLanes << "\n";
    for (const auto &vehicle: eastHeaded) {
        vehicle->print();
    }

    cout << "South bound " << intersection.southBoundLanes << "\n";
    for (const auto &vehicle: southHeaded) {
        vehicle->print();
    }

    cout << "West bound " << intersection.westBoundLanes << "\n";
    for (const auto &vehicle: westHeaded) {
        vehicle->print();
    }
}
