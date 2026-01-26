//
// Created by rusty on 11/5/2025.
//

#ifndef TRAFFICSIMULATOR_TRAFFIC_H
#define TRAFFICSIMULATOR_TRAFFIC_H

#include <atomic>
#include <chrono>

#include "vehicles.h"

constexpr bool greenLight = true;
constexpr bool redLight = false;

using namespace std;

struct TrafficLight {
    uint8_t greenLightTime = 0;

    uint8_t headingID = 0;

    //greenLight is true, reLight is false
    std::atomic<bool> currentLight{redLight};

    TrafficLight(const uint8_t headingID) : headingID(headingID) {
    }

    TrafficLight(TrafficLight &&other) noexcept
        : greenLightTime(other.greenLightTime),
          headingID(other.headingID) {
    }
};

struct Traffic {
    uint8_t speedMultiplier = 1;

    uint8_t speedLimit = 0;
    std::vector<TrafficLight *> lightsVector;

    TrafficLight northSouthLight{northSouth};

    uint8_t northBoundLanes = 0;

    TrafficLight eastWestLight{eastWest};

    uint8_t eastBoundLanes = 0;

    uint8_t southBoundLanes = 0;

    uint8_t westBoundLanes = 0;

    std::atomic<bool> running{false};

    // Traffic(const uint8_t speedLimit) : speedLimit(speedLimit){}

    void addLights();

    void trafficLoop();

    void trafficLightsLoop();

    uint8_t checkActiveLane();

    std::atomic<bool> goThroughLight{false};

    void passVehiclesThroughIntersection();

    void popVehicles(std::deque<std::unique_ptr<Vehicles> > &headingVector, uint8_t numberOfLanes, string direction);
};

uint8_t checkUserInput(int userInput);

void createIntersection(Traffic &intersection, uint8_t northSouthLanes, uint8_t eastWestLanes, uint8_t speed,
                        uint8_t northSouthLightTimer, uint8_t eastWestLightTimer);

void printIntersection(const Traffic &intersection);

void delay(int delayTimer, bool emergencyExit, std::chrono::seconds *timeRemaining);

extern std::chrono::seconds greenLightTimeRemaining;


#endif //TRAFFICSIMULATOR_TRAFFIC_H
