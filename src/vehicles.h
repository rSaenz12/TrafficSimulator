//********************************************
// Author: Russell Saenz
// File: vehicles.h
// Description: Declares Vehicle class and its child classes (Sedan, MiniVan, PickupTruck, SportsCar).
//              Declares functions useLock, populate, deques for NSEW directions, the mutex locks for the deques,
//              and atomic variables for tracking program stats
//********************************************

#pragma once

#include <iostream>

#include <vector>
#include <memory>
#include <deque>
#include <atomic>
#include <mutex>
#include <functional>

//directions and their IDs
constexpr uint8_t northSouth = 0;
constexpr uint8_t south = 0;
constexpr uint8_t eastWest = 2;
constexpr uint8_t west = 2;


class Vehicles {
protected:
    //size and weight
    // will be fixed values based on the type of vehicle
    //feet and pounds
    int weight;
    uint8_t length;
    uint8_t width;

    //speed
    //miles per hour
    uint8_t maximumSpeed; // will be fixed num based on type of vehicle
    // will be a range depending on the speed limit and max speed, statistics will determine it.
    uint8_t currentSpeed;

    //direction
    //1= north, 2= east,3= south, 4= west
    uint8_t heading;

    //misc
    uint8_t seats; // will be fixed num based on type of vehicle
    uint8_t passengers; // will be random 1 to seats
    bool distracted; // will be random based on statistics
    bool intoxicated; // will be random based on statistics
    bool crash;

public:
    //initializer chosen because these values default to 0, but will be changed for each child.
    Vehicles() : maximumSpeed(0), currentSpeed(0), length(0), width(0), weight(0), seats(0), passengers(0),
                 distracted(false), intoxicated(false), heading(0) {
    }

    virtual ~Vehicles() {
    }

    //check attributes
    int checkMaxSpeed() {
        return maximumSpeed;
    }

    int checkCurrentSpeed() {
        return currentSpeed;
    }

    int checkPassengers() {
        return passengers;
    }

    int checkSeats() {
        return seats;
    }

    bool checkDistracted() {
        return distracted;
    }

    bool checkIntoxicated() {
        return intoxicated;
    }

    int checkHeading() {
        return heading;
    }

    //set attributes that are randomly chosen
    void setCurrentSpeed(const uint8_t speed) {
        currentSpeed = speed;
    }

    void setPassengers(const uint8_t people) {
        passengers = people;
    }

    void setIntoxication(const bool intoxication) {
        intoxicated = intoxication;
    }

    void setDistraction(const bool distraction) {
        distracted = distraction;
    }

    void setCrash(const bool crashed) {
        crash = crashed;
    }

    void setHeading(const uint8_t direction) {
        heading = direction;
    }

    //print
    virtual void print() {
        std::cout << "Speed: " << static_cast<int>(currentSpeed) << "\n";
        std::cout << "Passengers:  " << static_cast<int>(passengers) << "\n";
        std::cout << "Intoxicated: " << static_cast<int>(intoxicated) << "\n";
        std::cout << "Distracted: " << static_cast<int>(distracted) << "\n";
        std::cout << "Direction: " << static_cast<int>(heading) << "\n";
        std::cout << "\n";
    }

    void createVehicles(uint8_t speed, bool isIntoxicated, bool isDistracted, uint8_t direction);

    bool crashDetection(uint8_t speedLimit);

    void removeVehicles();
};

class Sedan : public Vehicles {
public:
    Sedan() {
        maximumSpeed = 105;

        length = 14;
        width = 6;
        weight = 3700;

        seats = 5;
    }

    void print() override{
        std::cout << "Sedan" << std::endl;
        Vehicles::print();
    }
};

class PickupTruck : public Vehicles {
public:
    PickupTruck() {
        maximumSpeed = 95;

        length = 19;
        width = 7;
        weight = 5000;

        seats = 5;
    }

    void print() override{
        std::cout << "Pickup Truck" << std::endl;
        Vehicles::print();
    }
};

class MiniVan : public Vehicles {
public:
    MiniVan() {
        maximumSpeed = 80;

        length = 17;
        width = 7;
        weight = 4500;

        seats = 7;
    }

    void print() override{
        std::cout << "Mini van" << std::endl;
        Vehicles::print();
    }
};

class SportsCar : public Vehicles {
public:
    SportsCar() {
        maximumSpeed = 145;

        length = 16;
        width = 6;
        weight = 3000;

        seats = 2;
    }

    void print() override{
        std::cout << "Sports Car" << std::endl;
        Vehicles::print();
    }
};

//checks if deque is being written or read
void useLock(std::mutex &dequeMutex, const std::function<void()> &func);

//creates vehicles
void populate(uint8_t speedLimit);

//deques hold vehicles going in certain directions
extern std::deque<std::unique_ptr<Vehicles>> northHeaded; //1
extern std::deque<std::unique_ptr<Vehicles>> eastHeaded; //2
extern std::deque<std::unique_ptr<Vehicles>> southHeaded; //3
extern std::deque<std::unique_ptr<Vehicles>> westHeaded; //4

//locks for deques
extern std::mutex northMutex;
extern std::mutex southMutex;
extern std::mutex eastMutex;
extern std::mutex westMutex;

//these hold totals for whole program
extern std::atomic<uint8_t > intoxicatedDrivers;
extern std::atomic<uint8_t > distractedDrivers;
extern std::atomic<uint8_t > crashes;
extern std::atomic<uint16_t> totalDrivers;
extern std::atomic< uint16_t> totalPassengers;
extern std::atomic<uint32_t> totalWeight;
extern std::atomic<uint32_t> totalSpeed;
