//
// Created by rusty on 11/3/2025.
//

#ifndef TRAFFICSIMULATOR_VEHICLES_H
#define TRAFFICSIMULATOR_VEHICLES_H
#include <vector>



class vehicles {
protected:
    //speed
    //miles per hour
    int maximumSpeed; // will be fixed num based on type of vehicle
    // will be a range depending on the speed limit and max speed, statistics will determine it.
    int currentSpeed;

    //size and weight
    // will be fixed values based on the type of vehicle
    //feet and pounds
    int length;
    int width;
    int weight;

    //misc
    int seats; // will be fixed num based on type of vehicle
    int passengers; // will be random 1 to seats
    bool distracted; // will be random based on statistics
    bool intoxicated; // will be random based on statistics

    //direction
    //1= north, 2= east,3= south, 4= west
    int heading;

public:
    //initializer chosen because these values default to 0, but will be changed for each child.
    vehicles() : maximumSpeed(0), currentSpeed(0), length(0), width(0), weight(0), seats(0), passengers(0),
                distracted(false), intoxicated(false), heading(0) {
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
    void setCurrentSpeed(int speed) {
        currentSpeed = speed;
    }

    void setPassengers(int people) {
        passengers = people;
    }

    void setIntoxication(bool intoxication) {
        intoxicated = intoxication;
    }

    void setDistraction(bool distraction) {
        distracted = distraction;
    }

    void setHeading(int direction) {
        heading = direction;
    }

    //print
    void print() {
        std::cout<<"Speed: "<< currentSpeed<<std::endl;
        std::cout<<"Passengers:  "<<passengers<<std::endl;
        std::cout<<"Intoxicated: "<<intoxicated<<std::endl;
        std::cout<<"Distracted: "<<distracted<<std::endl;
        std::cout<<"Direction: "<<heading<<std::endl;
        std::cout<<std::endl;
    }

};

class sedan: public vehicles {
public:
    sedan() {
        maximumSpeed = 105;

        length = 14;
        width = 6;
        weight = 3700;

        seats = 5;
    }

    void print() {
        std::cout<<"Sedan"<<std::endl;
        vehicles::print();
    }



};

class pickupTruck: public vehicles {
public:
    pickupTruck() {
        maximumSpeed = 95;

        length = 19;
        width = 7;
        weight = 5000;

        seats = 5;
    }
    void print() {
        std::cout<<"Pickup Truck"<<std::endl;
        vehicles::print();
    }
};

 class miniVan: public vehicles {
 public:
     miniVan() {
         maximumSpeed = 80;

         length = 17;
         width = 7;
         weight = 4500;

         seats = 7;
     }
     void print() {
         std::cout<<"Mini van"<<std::endl;
         vehicles::print();
     }
 };

class sportsCar:public vehicles {
public:
    sportsCar() {
        maximumSpeed = 145;

        length = 16;
        width =6;
        weight= 3000;

        seats = 2;
    }

    void print() {
        std::cout<<"Sports Car"<<std::endl;
        vehicles::print();
    }
};

void populate(int);

std::vector<sedan> returnSedanArray();

std::vector<sportsCar> returnSportsCarArray();

std::vector<miniVan> returnMiniVanArray();

std::vector<pickupTruck> returnPickupTruck();

#endif //TRAFFICSIMULATOR_VEHICLES_H