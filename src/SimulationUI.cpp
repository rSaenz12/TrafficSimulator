//********************************************
// Author: Russell Saenz
// File: SimulationUI.cpp
// Description: Handles creating the UI elements. Draws roads, lines for roads, setup options,
//              traffic heatmaps,  and traffic statistics.
//********************************************
#include "SimulationUI.h"

#include <cmath>

using namespace ImGui;

void setupMenu(const ImGuiWindowFlags windowFlags, Traffic &intersection, int &northSouthLanes, int &eastWestLanes,
               int &speed, int &northSouthLightTimer, int &eastWestLightTimer) {
    //speed
    Begin("Speed Limit", nullptr, windowFlags);
        SliderInt("Speed", &speed, 1, 85);
    End();

    //lanes
    static int northSouthIndex = 2;
    static int eastWestIndex = 2;
    const char *laneOptions[] = {"1", "2", "3"};

    Begin("Lane Selection", nullptr, windowFlags);
        if (Combo("North/South Lanes", &northSouthIndex, laneOptions, IM_ARRAYSIZE(laneOptions))) {
            northSouthLanes = northSouthIndex + 1;
        }

        if (Combo("East/West Lanes", &eastWestIndex, laneOptions, IM_ARRAYSIZE(laneOptions))) {
            eastWestLanes = eastWestIndex + 1;
        }
    End();

    //light times
    Begin("North / South Light Timer");
        SliderInt("Seconds", &northSouthLightTimer, 1, 255);
    End();

    Begin("East / West Light Timer");
        SliderInt("Seconds", &eastWestLightTimer, 1, 255);
    End();

    speedSlider(windowFlags, intersection);
}

void buildRoad(const int northSouthLanes, const int eastWestLanes, const ImGuiWindowFlags windowFlags,
               Traffic &intersection) {
    SetNextWindowPos(ImVec2(250, 50), ImGuiCond_FirstUseEver);
    SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    Begin("Intersection", nullptr, windowFlags);

    const ImVec2 canvasPosition0 = GetCursorScreenPos();
    const ImVec2 canvasSpace = GetContentRegionAvail();
    const ImVec2 canvasPosition1 = ImVec2(canvasPosition0.x + canvasSpace.x, canvasPosition0.y + canvasSpace.y);
    ImDrawList *drawList = GetWindowDrawList();


    drawList->AddRectFilled(canvasPosition0, canvasPosition1, IM_COL32(34, 139, 34, 255));


    const float centerX = canvasPosition0.x + canvasSpace.x * 0.5f;
    const float centerY = canvasPosition0.y + canvasSpace.y * 0.5f;
    const float laneWidth = 40.0f;
    // float totalRoadWidth = laneWidth * lanes * 2;
    const float northSouthRoadWidth = laneWidth * northSouthLanes;
    const float eastWestRoadWidth = laneWidth * eastWestLanes;


    //void area in middle
    //horizontal left(x) to right(y)
    const ImVec2 horizontalVoid = ImVec2(centerX - northSouthRoadWidth / 2, centerX + northSouthRoadWidth / 2);
    //vertical top(x) to bottom(y)
    const ImVec2 verticalVoid = ImVec2(centerY - eastWestRoadWidth / 2, centerY + eastWestRoadWidth / 2);

    //road
    drawList->AddRectFilled(ImVec2(canvasPosition0.x, centerY - eastWestRoadWidth / 2),
                            ImVec2(canvasPosition1.x, centerY + eastWestRoadWidth / 2), IM_COL32(50, 50, 50, 255));
    drawList->AddRectFilled(ImVec2(centerX - northSouthRoadWidth / 2, canvasPosition0.y),
                            ImVec2(centerX + northSouthRoadWidth / 2, canvasPosition1.y), IM_COL32(50, 50, 50, 255));

    //yellow center lines
    ImU32 yellow = IM_COL32(255, 255, 0, 255);
    drawLine(drawList, ImVec2(canvasPosition0.x, centerY), horizontalVoid, ImVec2(canvasPosition1.x, centerY),
             yellow, 2.5f,
             0.0f); // Solid
    drawLine(drawList, ImVec2(centerX, canvasPosition0.y), verticalVoid, ImVec2(centerX, canvasPosition1.y),
             yellow, 2.5f,
             0.0f); // Solid

    //white lines
    drawLanesPerDirection(drawList, ImVec2(centerX, centerY), northSouthLanes, eastWestLanes, northSouthRoadWidth,
                          eastWestRoadWidth, canvasPosition0, canvasPosition1, horizontalVoid, verticalVoid);

    if (intersection.running) {
        drawTrafficGraphics(drawList, northSouthLanes, eastWestLanes, northSouthRoadWidth,
                            eastWestRoadWidth,
                            horizontalVoid, verticalVoid, centerX, centerY);
    }


    End();
}

//this function creates a colored button either red or green to represent the
void drawTrafficLightState(ImGuiWindowFlags windowFlags, Traffic &intersection) {
    const uint8_t currentLight = intersection.checkActiveLane();

    SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    SetNextWindowSize(ImVec2(100, 100), ImGuiCond_FirstUseEver);

    Begin("Light Status", nullptr, windowFlags);
        Text("North/South: ");
        SameLine();

        if (currentLight == northSouth) {
            PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
        } else {
            PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
        }

        Button("##red", ImVec2(20, 20));
        PopStyleColor();
        if (currentLight == northSouth && greenLightTimeRemaining > chrono::seconds(0)) {
            SameLine();
            Text("Time Remaining: %lld seconds", greenLightTimeRemaining);
        }

        Text("East/West: ");
        SameLine();
        if (currentLight == eastWest) {
            PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
        } else {
            PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
        }

        Button("##green", ImVec2(20, 20));
        PopStyleColor();
        if (currentLight == eastWest && greenLightTimeRemaining > chrono::seconds(0)) {
            SameLine();
            Text("Time Remaining: %lld seconds", greenLightTimeRemaining);
        }
    End();
}

//Handle the heat maps at the traffic lights
//those bars represent traffic building up
void drawTrafficGraphics(ImDrawList *drawList, const int northSouthLanes, const int eastWestLanes,
                         const float northSouthRoadWidth,
                         const float eastWestRoadWidth, const ImVec2 horizontalVoid, const ImVec2 verticalVoid,
                         const float centerX, const float centerY) {
    const float steps = 10.0f;

    const ImU32 green = IM_COL32(0, 255, 0, 50);
    const ImU32 yellow = IM_COL32(255, 255, 0, 50);
    const ImU32 red = IM_COL32(255, 0, 0, 50);
    ImU32 currentColor;

    int currentSize = 0;

    //grabbing the size of the deques
    int northHeadedSize;
    useLock(northMutex, [&] {
        northHeadedSize = northHeaded.size();
    });

    int southHeadedSize;
    useLock(southMutex, [&] {
        southHeadedSize = southHeaded.size();
    });

    int eastHeadedSize;
    useLock(eastMutex, [&] {
        eastHeadedSize = eastHeaded.size();
    });

    int westHeadedSize;
    useLock(westMutex, [&] {
        westHeadedSize = westHeaded.size();
    });

    //building the traffic heat bars
    //10+ = red,9 - 5 = yellow, <5 = green
    if (northHeadedSize > 0) {
        currentSize = ceil(northHeadedSize / northSouthLanes);

        if (currentSize >= 10) {
            currentColor = red;
        } else if (currentSize >= 5) {
            currentColor = yellow;
        } else {
            currentColor = green;
        }

        drawList->AddRectFilled(ImVec2(centerX, verticalVoid.y),
                                ImVec2(centerX + northSouthRoadWidth / 2, verticalVoid.y + (steps * currentSize)),
                                currentColor);
    }

    if (southHeadedSize > 0) {
        currentSize = ceil(southHeadedSize / northSouthLanes);

        if (currentSize >= 10) {
            currentColor = red;
        } else if (currentSize >= 5) {
            currentColor = yellow;
        } else {
            currentColor = green;
        }

        drawList->AddRectFilled(ImVec2(centerX, verticalVoid.x - (steps * currentSize)),
                                ImVec2(centerX - northSouthRoadWidth / 2, verticalVoid.x),
                                currentColor);
    }

    if (eastHeadedSize > 0) {
        currentSize = ceil(eastHeadedSize / eastWestLanes);


        if (currentSize >= 10) {
            currentColor = red;
        } else if (currentSize >= 5) {
            currentColor = yellow;
        } else {
            currentColor = green;
        }

        drawList->AddRectFilled(ImVec2(horizontalVoid.x - (steps * currentSize), centerY),
                                ImVec2(horizontalVoid.x, centerY + eastWestRoadWidth / 2),
                                currentColor);
    }

    if (westHeadedSize > 0) {
        currentSize = ceil(westHeadedSize / eastWestLanes);

        if (currentSize >= 10) {
            currentColor = red;
        } else if (currentSize >= 5) {
            currentColor = yellow;
        } else {
            currentColor = green;
        }

        drawList->AddRectFilled(ImVec2(horizontalVoid.y, centerY),
                                ImVec2(horizontalVoid.y + (steps * currentSize), centerY - eastWestRoadWidth / 2),
                                currentColor);
    }
}

void drawLanesPerDirection(ImDrawList *drawList, const ImVec2 center, const int northSouthLanes,
                           const int eastWestLanes, const float northSouthRoadWidth, const float eastWestRoadWidth,
                           const ImVec2 canvasPosition0, const ImVec2 canvasPosition1, const ImVec2 horizontalVoid,
                           const ImVec2 verticalVoid) {
    const ImU32 white = IM_COL32(255, 255, 255, 180);
    const float northSouthLaneDistance = northSouthRoadWidth / (northSouthLanes * 2);
    const float eastWestLaneDistance = eastWestRoadWidth / (eastWestLanes * 2);

    int lanes = northSouthLanes >= eastWestLanes ? northSouthLanes : eastWestLanes;

    //For this loop, lanes is whichever direction has most lanes. then the if statements check if the directions have the i lanes
    // within the if we draw a dashed lane line per direction using offset +/- center
    for (int i = 1; i < lanes; i++) {
        //Vertical Lane Markers (North/South)
        if (i < northSouthLanes) {
            float northSouthOffset = i * northSouthLaneDistance;

            drawLine(drawList, ImVec2(center.x + northSouthOffset, canvasPosition0.y), verticalVoid,
                     ImVec2(center.x + northSouthOffset, canvasPosition1.y), white, 1.0f, 15.0f);
            drawLine(drawList, ImVec2(center.x - northSouthOffset, canvasPosition0.y), verticalVoid,
                     ImVec2(center.x - northSouthOffset, canvasPosition1.y), white, 1.0f, 15.0f);
        }

        //Horizontal Lane Markers (East/West)
        if (i < eastWestLanes) {
            float eastWestOffset = i * eastWestLaneDistance;

            drawLine(drawList, ImVec2(canvasPosition0.x, center.y + eastWestOffset), horizontalVoid,
                     ImVec2(canvasPosition1.x, center.y + eastWestOffset), white, 1.0f, 15.0f);
            drawLine(drawList, ImVec2(canvasPosition0.x, center.y - eastWestOffset), horizontalVoid,
                     ImVec2(canvasPosition1.x, center.y - eastWestOffset), white, 1.0f, 15.0f);
        }
    }
}

//Draws a line from start to end
//Void area allows there to be no lines in the center of the intersection
void drawLine(ImDrawList *drawList, const ImVec2 start, const ImVec2 voidArea, const ImVec2 end,
              const ImU32 color, const float thickness, const float dashLength) {
    const ImVec2 difference = {end.x - start.x, end.y - start.y};
    const float length = sqrtf(difference.x * difference.x + difference.y * difference.y);
    if (length <= 0.0f) {
        return;
    }

    const ImVec2 direction = {difference.x / length, difference.y / length};
    const bool isVertical = (fabs(direction.y) > fabs(direction.x));

    //solid
    if (dashLength <= 0.0f) {
        //start to void
        const ImVec2 voidEntry = isVertical ? ImVec2(start.x, voidArea.x) : ImVec2(voidArea.x, start.y);
        drawList->AddLine(start, voidEntry, color, thickness);

        //void to end
        const ImVec2 voidExit = isVertical ? ImVec2(start.x, voidArea.y) : ImVec2(voidArea.y, start.y);
        drawList->AddLine(voidExit, end, color, thickness);

        return;
    }

    //dashed
    for (float i = 0; i < length; i += dashLength * 2) {
        const float startDistance = i;
        const float endDistance = (i + dashLength > length) ? length : i + dashLength;

        const ImVec2 startPosition = ImVec2(start.x + direction.x * startDistance,
                                            start.y + direction.y * startDistance);
        const ImVec2 endPosition = ImVec2(start.x + direction.x * endDistance, start.y + direction.y * endDistance);

        const float checkCoordinate = isVertical ? startPosition.y : startPosition.x;

        //skips over void area
        if (checkCoordinate > voidArea.x && checkCoordinate < voidArea.y) {
            continue;
        }

        drawList->AddLine(startPosition, endPosition, color, thickness);
    }
}

void displayStats(const ImGuiWindowFlags windowFlags, const Traffic &intersection) {
    //grabbing the size of the deques
    int northHeadedSize;
    useLock(northMutex, [&] {
        northHeadedSize = northHeaded.size();
    });

    int southHeadedSize;
    useLock(southMutex, [&] {
        southHeadedSize = southHeaded.size();
    });

    int eastHeadedSize;
    useLock(eastMutex, [&] {
        eastHeadedSize = eastHeaded.size();
    });

    int westHeadedSize;
    useLock(westMutex, [&] {
        westHeadedSize = westHeaded.size();
    });

    //UI
    Begin("Traffic Stats", nullptr, windowFlags);
        Text("Speed Limit: %u MPH", intersection.speedLimit);

        Text("North/South Light Duration: %u seconds", intersection.northSouthLight.greenLightTime);

        Text("East/West Light Duration: %u seconds", intersection.eastWestLight.greenLightTime);

        Text("Cars Headed North: %d", northHeadedSize);

        Text("Cars Headed South: %d", southHeadedSize);

        Text("Cars Headed East: %d", eastHeadedSize);

        Text("Cars Headed West: %d", westHeadedSize);

        Text("Total Vehicles: %u", totalDrivers.load());

        Text("Intoxicated Drivers: %u", intoxicatedDrivers.load());

        Text("Distracted Drivers: %u", distractedDrivers.load());

        Text("Accidents: %u", crashes.load());

        //added inline ifs to avoid dividing by 0
        Text("Total Passengers: %u, Passengers per Vehicle: %d ", totalPassengers.load(),
             static_cast<int>(totalPassengers.load() / (totalDrivers.load() == 0 ? 1 : totalDrivers.load())));

        Text("Total Weight of Vehicles: %u lbs, Average Vehicle Weight: %d lbs", totalWeight.load(),
             static_cast<int>(totalWeight / (totalDrivers.load() == 0 ? 1 : totalDrivers.load())));

        Text("Average Speed of Vehicles: %d MPH",
             static_cast<int>(totalSpeed.load() / (totalDrivers.load() == 0 ? 1 : totalDrivers.load())));
    End();
}

void speedSlider(const ImGuiWindowFlags windowFlags, Traffic &intersection) {
    int speed = intersection.speedMultiplier;

    Begin("Speed Multiplier", nullptr, windowFlags);
        SliderInt("Multiplier", &speed, 1, 5);
    End();

    intersection.speedMultiplier = speed;
}
