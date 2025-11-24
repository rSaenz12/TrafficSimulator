//
// Created by rusty on 11/3/2025.
//

#include <iostream>
#include "vehicles.h"
#include <random>

using namespace std;

vector<sedan> sedansArray;
vector<sportsCar> sportsCarsArray;
vector<miniVan> miniVanArray;
vector<pickupTruck> pickupTruckArray;

void populate(int speedLimit) {

    int carType = rand() % 100;
    int speedFactor = rand() % 100;
    int speed = 0;

    cout << carType << endl;

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

    //sedans make up 52% of cars
    if (carType < 53) {
        sedan sedan;
        sedan.setCurrentSpeed(speed);
        sedan.setPassengers(rand() % sedan.checkSeats() + 1);
        sedan.setIntoxication(intoxicated);
        sedan.setDistraction(distracted);
        sedan.setHeading(heading);
        //sedan.print();
        sedansArray.push_back(sedan);
    } else if (carType < 56) {
        //sports cars make up about 3%
        sportsCar sportsCar;
        sportsCar.setCurrentSpeed(speed);
        sportsCar.setPassengers(rand() % sportsCar.checkSeats() + 1);
        sportsCar.setIntoxication(intoxicated);
        sportsCar.setDistraction(distracted);
        sportsCar.setHeading(heading);
        //sportsCar.print();
        sportsCarsArray.push_back(sportsCar);
    } else if (carType < 65) {
        //minivans make up about 9%
        miniVan miniVan;
        miniVan.setCurrentSpeed(speed);
        miniVan.setPassengers(rand() % miniVan.checkSeats() + 1);
        miniVan.setIntoxication(intoxicated);
        miniVan.setDistraction(distracted);
        miniVan.setHeading(heading);
        //miniVan.print();
        miniVanArray.push_back(miniVan);
    } else {
        //pickup trucks make 36%
        pickupTruck pickupTruck;
        pickupTruck.setCurrentSpeed(speed);
        pickupTruck.setPassengers(rand() % pickupTruck.checkSeats() + 1);
        pickupTruck.setIntoxication(intoxicated);
        pickupTruck.setDistraction(distracted);
        pickupTruck.setHeading(heading);
        //pickupTruck.print();
        pickupTruckArray.push_back(pickupTruck);
    }
}

vector<sedan> returnSedanArray() {
    return sedansArray;
}

vector<sportsCar> returnSportsCarArray() {
    return sportsCarsArray;
}

vector<miniVan> returnMiniVanArray() {
    return miniVanArray;
}

vector<pickupTruck> returnPickupTruck() {
    return pickupTruckArray;
}
