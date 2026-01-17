//
// Created by rusty on 11/5/2025.
//

#ifndef TRAFFICSIMULATOR_TRAFFIC_H
#define TRAFFICSIMULATOR_TRAFFIC_H
#include <atomic>

#include "vehicles.h"

constexpr bool greenLight = true;
constexpr bool redLight = false;

using namespace std;

struct TrafficLight {
    std::chrono::seconds greenLightTime =  static_cast<std::chrono::seconds>(0);

    int headingID = 0;

    //greenLight is true, reLight is false
    std::atomic<bool> currentLight {redLight};

    TrafficLight(const int headingID) : headingID(headingID){}

    TrafficLight(TrafficLight&& other) noexcept
    : greenLightTime(other.greenLightTime),
          headingID(other.headingID) {}
};

struct Traffic {
    int speedLimit = 0;
    std::vector<TrafficLight*> lightsVector;

    TrafficLight northSouthLight{northSouth};
    //left turns
    TrafficLight northSouthTurnLight{northSouthLeft};

    int northBoundLanes = 0;
    int northLeftTurnLanes = 0;
    int northRightTurnLanes = 0;

    TrafficLight eastWestLight{eastWest};
    //lect turns
    TrafficLight eastWestTurnLight{eastWestLeft};

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

    void addLights();

    void trafficLoop();
    void trafficLightsLoop();

    int checkActiveLane();

    std::atomic<bool> goThroughLight {false};

    void passVehiclesThroughIntersection();
    void popVehicles(std::vector<std::unique_ptr<Vehicles>> &headingVector, int numberOfLanes, string direction);
};

void checkUserInput(int &userInput);

void createIntersection(Traffic &intersection);

void printIntersection(const Traffic &intersection);

void delay(int delayTimer, bool emergencyExit);




#endif //TRAFFICSIMULATOR_TRAFFIC_H