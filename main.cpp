#include <iostream>
#include <vector>

#include <bits/this_thread_sleep.h>

#include "vehicles.h"


using namespace std;
int main() {
    srand((time(nullptr)));
    int counter =0;
    while (true) {
        populate(45);
        //this_thread::sleep_for(chrono::milliseconds(500));
        counter++;

        if (counter>=5) {
            break;
        }
    }

    vector<sedan> sedanArray = returnSedanArray();
    vector<sportsCar> sportsCarArray = returnSportsCarArray();
    vector<miniVan> miniVanArray = returnMiniVanArray();
    vector<pickupTruck> pickupTruckArray = returnPickupTruck();

    for (auto &sedan:sedanArray) {
        sedan.print();
    }
    for (auto &sportsCar:sportsCarArray) {
        sportsCar.print();
    }
    for (auto &miniVan:miniVanArray) {
        miniVan.print();
    }
    for (auto &pickupTruck:pickupTruckArray) {
        pickupTruck.print();
    }

    return 0;


}