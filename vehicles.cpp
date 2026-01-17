//
// Created by rusty on 11/3/2025.
//

#include <iostream>
#include "vehicles.h"
#include <random>

using namespace std;

std::vector<std::unique_ptr<Vehicles>> northHeaded; //1
std::vector<std::unique_ptr<Vehicles>> eastHeaded; //2
std::vector<std::unique_ptr<Vehicles>> southHeaded; //3
std::vector<std::unique_ptr<Vehicles>> westHeaded; //4



void Vehicles::createVehicles(const int speed, const bool isIntoxicated, const bool isDistracted, const int direction) {
    setCurrentSpeed(speed);
    setPassengers(rand() % checkSeats() + 1);
    setIntoxication(isIntoxicated);
    setDistraction(isDistracted);
    setHeading(direction);
}

bool Vehicles::crashDetection(const int speedLimit) {
    //standard chance 0.5%
    //dist 1.5x
    //dist+speed 10-15x
    //intox + dist 10-15x
    //intox + speed 15-25x
    //intox + dist + speed 50x

    double crashChance = 0.5;

    const double risk = (rand()%1000)/10.0;

    if (intoxicated && distracted) {
        crashChance *= 50.0;
    }else if (intoxicated) {
        crashChance *= 25.0;
    }else if (distracted) {
        crashChance *= 15.0;
    }

    if (currentSpeed > speedLimit) {
        crashChance *= 2.0;
    }


    return risk<=crashChance;
}

void populate(const int speedLimit) {

    int carType = rand() % 100;
    int speedFactor = rand() % 100;
    int speed = 0;

    cout << carType << "car type \n";

    int intoxicatedChance = rand() % 100;
    bool intoxicated = false;
    int distractedChance = rand() % 100;
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
    }

    //estimated around 10% of drivers are
    if (distractedChance <= 10) {
        distracted = true;
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

