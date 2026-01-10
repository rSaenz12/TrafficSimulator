#include <iostream>
#include <vector>

#include <bits/this_thread_sleep.h>

#include "traffic.h"
#include "vehicles.h"


using namespace std;
int main() {
    srand((time(nullptr)));

    Traffic intersection(45);
    createIntersection(intersection);

    for (int i=0; i<5; i++){
        populate(intersection.speedLimit);
        //this_thread::sleep_for(chrono::milliseconds(500));
    }

    printIntersection(intersection);

    return 0;
}