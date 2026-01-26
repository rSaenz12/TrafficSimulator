//
// Created by rusty on 11/3/2025.
//

#include <iostream>
#include "vehicles.h"
#include <random>

using namespace std;

std::deque<std::unique_ptr<Vehicles> > northHeaded; //1
std::deque<std::unique_ptr<Vehicles> > eastHeaded; //2
std::deque<std::unique_ptr<Vehicles> > southHeaded; //3
std::deque<std::unique_ptr<Vehicles> > westHeaded; //4

//totals of sim
uint8_t intoxicatedDrivers = 0;
uint8_t distractedDrivers = 0;
uint8_t crashes = 0;

//also doubles as total cars
uint16_t totalDrivers = 0;
uint16_t totalPassengers = 0;
uint32_t totalWeight = 0;

//will be used a totalSpeed/totalDrivers = average speed
uint32_t totalSpeed = 0;


void Vehicles::createVehicles(const uint8_t speed, const bool isIntoxicated, const bool isDistracted,
                              const uint8_t direction) {
    setCurrentSpeed(speed);
    setPassengers(rand() % checkSeats() + 1);
    setIntoxication(isIntoxicated);
    setDistraction(isDistracted);
    setCrash(crashDetection(speed));
    setHeading(direction);

    //updating totals
    totalDrivers ++;
    //taking advantage of bool being 1 or 0
    intoxicatedDrivers += intoxicated;
    distractedDrivers += distracted;

    crashes += crash;
    totalPassengers += passengers;
    totalWeight += weight;
    totalSpeed += currentSpeed;
}

//checks if a car is crashing, probably run as car is going
bool Vehicles::crashDetection(const uint8_t speedLimit) {
    //standard chance 0.5%
    //dist 1.5x
    //dist+speed 10-15x
    //intox + dist 10-15x
    //intox + speed 15-25x
    //intox + dist + speed 50x

    double crashChance = 0.5;

    const double risk = (rand() % 1000) / 10.0;

    if (intoxicated && distracted) {
        crashChance *= 50.0;
    } else if (intoxicated) {
        crashChance *= 25.0;
    } else if (distracted) {
        crashChance *= 15.0;
    }

    if (currentSpeed > speedLimit) {
        crashChance *= 2.0;
    }

    // const bool isCrashed = risk <= crashChance;
    // if (isCrashed) {
    //     crashes++;
    // }
    //
    // return isCrashed;

    return  risk <= crashChance;
}

void populate(const uint8_t speedLimit) {

    const int carType = rand() % 100;
    const int speedFactor = rand() % 100;
    int speed = 0;

    // cout << carType << "car type \n";

    const int intoxicatedChance = rand() % 100;
    bool intoxicated = false;
    const int distractedChance = rand() % 100;
    bool distracted = false;

    int heading = rand() % 4 + 1;

    //upper and lower bounds for fast and slow drivers, the middle would be those who drive the speed limit.
    if (speedFactor < 60) {
        //59% of drivers speed
        //way to increase range
        if (rand() % 2 == 0) {
            speed = speedLimit + rand() % 5 + 1;
        } else {
            speed = speedLimit + rand() % 15 + 5;
        }
    } else if (speedFactor > 76) {
        //way to diversify the range of how slow they can drive
        if (rand() % 2 == 0) {
            speed = speedLimit - rand() % 5 + 1;
        } else {
            speed = speedLimit - rand() % 10 + 5;
        }
    } else {
        speed = speedLimit;
    }

    if (intoxicatedChance <= 3) {
        intoxicated = true;
        // intoxicatedDrivers++;
    }

    //estimated around 10% of drivers are
    if (distractedChance <= 10) {
        distracted = true;
        // distractedDrivers++;
    }

    unique_ptr<Vehicles> vehicle;

    //sedans make up 52% of cars
    if (carType < 53) {
        vehicle = make_unique<Sedan>();
    } else if (carType < 56) {
        //sports cars make up about 3%
        vehicle = make_unique<SportsCar>();
    } else if (carType < 65) {
        //minivans make up about 9%
        vehicle = make_unique<MiniVan>();
    } else {
        //pickup trucks make 36%
        vehicle = make_unique<PickupTruck>();
    }

    vehicle->createVehicles(speed, intoxicated, distracted, heading);

    switch (heading) {
        case 1: {
            northHeaded.push_back(move(vehicle));
            break;
        }
        case 2: {
            eastHeaded.push_back(move(vehicle));
            break;
        }
        case 3: {
            southHeaded.push_back(move(vehicle));
            break;
        }
        case 4: {
            westHeaded.push_back(move(vehicle));
            break;
        }
    }
}
