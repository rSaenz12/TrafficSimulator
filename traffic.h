//
// Created by rusty on 11/5/2025.
//

#ifndef TRAFFICSIMULATOR_TRAFFIC_H
#define TRAFFICSIMULATOR_TRAFFIC_H
#include <atomic>

#include "vehicles.h"

constexpr bool greenLight = true;
constexpr bool redLight = false;

struct TrafficLight {
    std::chrono::seconds greenLightTime =  static_cast<std::chrono::seconds>(0);

    //greenLight is true, reLight is false
    std::atomic<bool> currentLight {redLight};
};

struct Traffic {
    int speedLimit = 0;

    TrafficLight northSouthLight;
    TrafficLight northSouthTurnLight;

    int northBoundLanes = 0;
    int northLeftTurnLanes = 0;
    int northRightTurnLanes = 0;

    TrafficLight eastWestLight;
    TrafficLight eastWestTurnLight;

    int eastBoundLanes = 0;
    int eastLeftTurnLanes = 0;
    int eastRightTurnLanes = 0;

    int southBoundLanes = 0;
    int southLeftTurnLanes = 0;
    int southRightTurnLanes = 0;

    int westBoundLanes = 0;
    int westLeftTurnLanes = 0;
    int westRightTurnLanes = 0;

    std::atomic<bool> running {false};

    Traffic(const int speedLimit) : speedLimit(speedLimit){}

    void trafficLoop();
    void trafficLightsLoop();
};

void createIntersection(Traffic &intersection);

void printIntersection(const Traffic &intersection);




#endif //TRAFFICSIMULATOR_TRAFFIC_H